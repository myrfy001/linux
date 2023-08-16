
#include <rdma/ib_verbs.h>

#include "dtld.h"


MODULE_AUTHOR("Eli Cohen <eli@mellanox.com>");
MODULE_DESCRIPTION("Mellanox 5th generation network adapters (ConnectX series) IB driver");
MODULE_LICENSE("Dual BSD/GPL");




/* initialize dtld device parameters */
static void dtld_init_device_param(struct dtld_dev *dtld)
{

	dtld->attr.vendor_id			= DTLD_VENDOR_ID;
	dtld->attr.max_mr_size			= DTLD_MAX_MR_SIZE;
	dtld->attr.page_size_cap			= DTLD_PAGE_SIZE_CAP;
	dtld->attr.max_qp			= DTLD_MAX_QP;
	dtld->attr.max_qp_wr			= DTLD_MAX_QP_WR;
	dtld->attr.device_cap_flags		= DTLD_DEVICE_CAP_FLAGS;
	dtld->attr.kernel_cap_flags		= IBK_ALLOW_USER_UNREG;
	dtld->attr.max_send_sge			= DTLD_MAX_SGE;
	dtld->attr.max_recv_sge			= DTLD_MAX_SGE;
	dtld->attr.max_sge_rd			= DTLD_MAX_SGE_RD;
	dtld->attr.max_cq			= DTLD_MAX_CQ;
	dtld->attr.max_cqe			= (1 << DTLD_MAX_LOG_CQE) - 1;
	dtld->attr.max_mr			= DTLD_MAX_MR;
	dtld->attr.max_mw			= DTLD_MAX_MW;
	dtld->attr.max_pd			= DTLD_MAX_PD;
	dtld->attr.max_qp_rd_atom		= DTLD_MAX_QP_RD_ATOM;
	dtld->attr.max_res_rd_atom		= DTLD_MAX_RES_RD_ATOM;
	dtld->attr.max_qp_init_rd_atom		= DTLD_MAX_QP_INIT_RD_ATOM;
	dtld->attr.atomic_cap			= IB_ATOMIC_HCA;
	dtld->attr.max_mcast_grp			= DTLD_MAX_MCAST_GRP;
	dtld->attr.max_mcast_qp_attach		= DTLD_MAX_MCAST_QP_ATTACH;
	dtld->attr.max_total_mcast_qp_attach	= DTLD_MAX_TOT_MCAST_QP_ATTACH;
	dtld->attr.max_ah			= DTLD_MAX_AH;
	dtld->attr.max_srq			= DTLD_MAX_SRQ;
	dtld->attr.max_srq_wr			= DTLD_MAX_SRQ_WR;
	dtld->attr.max_srq_sge			= DTLD_MAX_SRQ_SGE;
	dtld->attr.max_fast_reg_page_list_len	= DTLD_MAX_FMR_PAGE_LIST_LEN;
	dtld->attr.max_pkeys			= DTLD_MAX_PKEYS;
	dtld->attr.local_ca_ack_delay		= DTLD_LOCAL_CA_ACK_DELAY;
	// addrconf_addr_eui48((unsigned char *)&dtld->attr.sys_image_guid,
	// 		dtld->ndev->dev_addr);

	// dtld->max_ucontext			= DTLD_MAX_UCONTEXT;
}

/* initialize port attributes */
static void dtld_init_port_param(struct dtld_port *port)
{
	port->attr.state		= IB_PORT_DOWN;
	port->attr.max_mtu		= IB_MTU_4096;
	port->attr.active_mtu		= IB_MTU_256;
	port->attr.gid_tbl_len		= DTLD_PORT_GID_TBL_LEN;
	port->attr.port_cap_flags	= DTLD_PORT_PORT_CAP_FLAGS;
	port->attr.max_msg_sz		= DTLD_PORT_MAX_MSG_SZ;
	port->attr.bad_pkey_cntr	= DTLD_PORT_BAD_PKEY_CNTR;
	port->attr.qkey_viol_cntr	= DTLD_PORT_QKEY_VIOL_CNTR;
	port->attr.pkey_tbl_len		= DTLD_PORT_PKEY_TBL_LEN;
	port->attr.lid			= DTLD_PORT_LID;
	port->attr.sm_lid		= DTLD_PORT_SM_LID;
	port->attr.lmc			= DTLD_PORT_LMC;
	port->attr.max_vl_num		= DTLD_PORT_MAX_VL_NUM;
	port->attr.sm_sl		= DTLD_PORT_SM_SL;
	port->attr.subnet_timeout	= DTLD_PORT_SUBNET_TIMEOUT;
	port->attr.init_type_reply	= DTLD_PORT_INIT_TYPE_REPLY;
	port->attr.active_width		= DTLD_PORT_ACTIVE_WIDTH;
	port->attr.active_speed		= DTLD_PORT_ACTIVE_SPEED;
	port->attr.phys_state		= DTLD_PORT_PHYS_STATE;
	port->mtu_cap			= ib_mtu_enum_to_int(IB_MTU_256);
	port->subnet_prefix		= cpu_to_be64(DTLD_PORT_SUBNET_PREFIX);
}

static void dtld_init_ports(struct dtld_dev *dtld)
{
	struct dtld_port *port = &dtld->port;

	dtld_init_port_param(port);
	// addrconf_addr_eui48((unsigned char *)&port->port_guid,
	// 		    dtld->ndev->dev_addr);
	spin_lock_init(&port->port_lock);
}

void dtld_set_mtu(struct dtld_dev *dtld, unsigned int ndev_mtu)
{
	struct dtld_port *port = &dtld->port;
	enum ib_mtu mtu;

	mtu = eth_mtu_int_to_enum(ndev_mtu);

	/* Make sure that new MTU in range */
	mtu = mtu ? min_t(enum ib_mtu, mtu, IB_MTU_4096) : IB_MTU_256;

	port->attr.active_mtu = mtu;
	port->mtu_cap = ib_mtu_enum_to_int(mtu);
}


static int __init dtld_ib_init(void)
{
	int err;

	struct dtld_dev *dtld = NULL;

	dtld = ib_alloc_device(dtld_dev, ib_dev);

	dtld_init_device_param(dtld);

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
