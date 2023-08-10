#ifndef DTLD_VERBS_H
#define DTLD_VERBS_H

#include <rdma/ib_verbs.h>

struct dtld_dev {
	struct ib_device	ib_dev;
	struct ib_device_attr	attr;
};

static inline struct dtld_dev *dtld_from_ibdev(struct ib_device *dev)
{
	return dev ? container_of(dev, struct dtld_dev, ib_dev) : NULL;
}

int dtld_register_device(struct dtld_dev *dtld, const char *ibdev_name);
void dtld_unregister_device(struct dtld_dev *dtld);

#endif