#include "common.h"
#include "kernel.h"
#include "kernel_io.h"

uint32_t virtio_reg_read32(unsigned offset)
{
    return *((volatile uint32_t *)(VIRTIO_BLK_PADDR + offset));
}

uint64_t virtio_reg_read64(unsigned offset)
{
    return *((volatile uint64_t *)(VIRTIO_BLK_PADDR + offset));
}

void virtio_reg_write32(unsigned offset, uint32_t value)
{
    *((volatile uint32_t *)(VIRTIO_BLK_PADDR + offset)) = value;
}

void virtio_reg_fetch_and_or32(unsigned offset, uint32_t value)
{
    virtio_reg_write32(offset, virtio_reg_read32(offset) | value);
}

struct virtio_virtq *blk_request_vq;
struct virtio_blk_req *blk_req;
paddr_t blk_req_paddr;
unsigned blk_capacity;

/**
 * https://docs.oasis-open.org/virtio/virtio/v1.1/csprd01/virtio-v1.1-csprd01.html#x1-910003
 */
void virtio_blk_init(void)
{
    if (virtio_reg_read32(VIRTIO_REG_MAGIC) != 0x74726976)
        PANIC("virtio: invalid magic value");
    if (virtio_reg_read32(VIRTIO_REG_VERSION) != 1)
        PANIC("virtio: invalid version");
    if (virtio_reg_read32(VIRTIO_REG_DEVICE_ID) != VIRTIO_DEVICE_BLK)
        PANIC("virtio: invalid device id");

    // 1. 重置设备。
    virtio_reg_write32(VIRTIO_REG_DEVICE_STATUS, 0);
    // 2. 设置 ACKNOWLEDGE 状态位：表示客户操作系统已注意到该设备。
    virtio_reg_fetch_and_or32(VIRTIO_REG_DEVICE_STATUS, VIRTIO_STATUS_ACK);
    // 3. 设置 DRIVER 状态位。
    virtio_reg_fetch_and_or32(VIRTIO_REG_DEVICE_STATUS, VIRTIO_STATUS_DRIVER);
    // 5. 设置 FEATURES_OK 状态位。
    virtio_reg_fetch_and_or32(VIRTIO_REG_DEVICE_STATUS, VIRTIO_STATUS_FEAT_OK);
    // 7. 执行设备特定的设置，包括发现设备的 virtqueues
    blk_request_vq = virtq_init(0);
    // 8. 设置 DRIVER_OK 状态位。
    virtio_reg_write32(VIRTIO_REG_DEVICE_STATUS, VIRTIO_STATUS_DRIVER_OK);

    // 获取磁盘容量。
    blk_capacity = virtio_reg_read64(VIRTIO_REG_DEVICE_CONFIG + 0) * SECTOR_SIZE;
    printf("virtio-blk: capacity is %d bytes\n", blk_capacity);

    // 分配一个区域来存储对设备的请求。
    blk_req_paddr = alloc_pages(align_up(sizeof(*blk_req), PAGE_SIZE) / PAGE_SIZE);
    blk_req = (struct virtio_blk_req *)blk_req_paddr;
}

struct virtio_virtq *virtq_init(unsigned index)
{
    // 为 virtqueue 分配一个区域。
    paddr_t virtq_paddr = alloc_pages(align_up(sizeof(struct virtio_virtq), PAGE_SIZE) / PAGE_SIZE);
    struct virtio_virtq *vq = (struct virtio_virtq *)virtq_paddr;
    vq->queue_index = index;
    vq->used_index = (volatile uint16_t *)&vq->used.index;
    // 1. 通过写入队列的索引(第一个队列为 0)到 QueueSel 来选择队列。
    virtio_reg_write32(VIRTIO_REG_QUEUE_SEL, index);
    // 5. 通过写入大小到 QueueNum 来通知设备队列大小。
    virtio_reg_write32(VIRTIO_REG_QUEUE_NUM, VIRTQ_ENTRY_NUM);
    // 6. 通过写入其字节值到 QueueAlign 来通知设备已使用的对齐方式。
    virtio_reg_write32(VIRTIO_REG_QUEUE_ALIGN, 0);
    // 7. 将队列第一页的物理编号写入 QueuePFN 寄存器。
    virtio_reg_write32(VIRTIO_REG_QUEUE_PFN, virtq_paddr);
    return vq;
}

// 通知设备有新的请求。`desc_index` 是新请求头描述符的索引。
void virtq_kick(struct virtio_virtq *vq, int desc_index)
{
    vq->avail.ring[vq->avail.index % VIRTQ_ENTRY_NUM] = desc_index;
    vq->avail.index++;
    __sync_synchronize();
    virtio_reg_write32(VIRTIO_REG_QUEUE_NOTIFY, vq->queue_index);
    vq->last_used_index++;
}

// 返回是否有请求正在被设备处理。
bool virtq_is_busy(struct virtio_virtq *vq)
{
    return vq->last_used_index != *vq->used_index;
}

// 从 virtio-blk 设备读取/写入。
void read_write_disk(void *buf, unsigned sector, int is_write)
{
    if (sector >= blk_capacity / SECTOR_SIZE)
    {
        printf("virtio: tried to read/write sector=%d, but capacity is %d\n",
               sector, blk_capacity / SECTOR_SIZE);
        return;
    }

    // 根据 virtio-blk 规范构造请求。
    blk_req->sector = sector;
    blk_req->type = is_write ? VIRTIO_BLK_T_OUT : VIRTIO_BLK_T_IN;
    if (is_write)
        memcpy(blk_req->data, buf, SECTOR_SIZE);

    // 构造 virtqueue 描述符(使用 3 个描述符)。
    struct virtio_virtq *vq = blk_request_vq;
    vq->descs[0].addr = blk_req_paddr;
    vq->descs[0].len = sizeof(uint32_t) * 2 + sizeof(uint64_t);
    vq->descs[0].flags = VIRTQ_DESC_F_NEXT;
    vq->descs[0].next = 1;

    vq->descs[1].addr = blk_req_paddr + offsetof(struct virtio_blk_req, data);
    vq->descs[1].len = SECTOR_SIZE;
    vq->descs[1].flags = VIRTQ_DESC_F_NEXT | (is_write ? 0 : VIRTQ_DESC_F_WRITE);
    vq->descs[1].next = 2;

    vq->descs[2].addr = blk_req_paddr + offsetof(struct virtio_blk_req, status);
    vq->descs[2].len = sizeof(uint8_t);
    vq->descs[2].flags = VIRTQ_DESC_F_WRITE;

    // 通知设备有新的请求。
    virtq_kick(vq, 0);

    // 等待设备完成处理。
    while (virtq_is_busy(vq))
        ;

    // virtio-blk：如果返回非零值，则表示错误。
    if (blk_req->status != 0)
    {
        printf("virtio: warn: failed to read/write sector=%d status=%d\n",
               sector, blk_req->status);
        return;
    }

    // 对于读操作，将数据复制到缓冲区。
    if (!is_write)
        memcpy(buf, blk_req->data, SECTOR_SIZE);
}