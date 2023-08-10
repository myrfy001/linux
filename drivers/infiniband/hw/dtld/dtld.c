
#include <rdma/ib_verbs.h>

#include "dtld_verbs.h"

MODULE_AUTHOR("Eli Cohen <eli@mellanox.com>");
MODULE_DESCRIPTION("Mellanox 5th generation network adapters (ConnectX series) IB driver");
MODULE_LICENSE("Dual BSD/GPL");


static int __init dtld_ib_init(void)
{
	int err;

	struct dtld_dev *dtld = NULL;

	dtld = ib_alloc_device(dtld_dev, ib_dev);

	err = dtld_register_device(dtld, "dtld-dev");

	if (err){
		pr_warn("%s failed with error %d\n", __func__, err);
		ib_dealloc_device(&dtld->ib_dev);
	}
		
	return 0;
}

static void __exit dtld_ib_cleanup(void)
{
	
}

module_init(dtld_ib_init);
module_exit(dtld_ib_cleanup);
