
enum DMA0_Regiters {
	DMA0_Status_Register = 0x08,
	DMA0_Command_Register = 0x08,
	DMA0_Request_Register = 0x09,
	DMA0_Single_Mask_Register = 0x0A,
	DMA0_Mode_Register = 0x0B,
	DMA0_Clear_Byte_FlipFlop_Register = 0x0C,
	DMA0_Intermediate_Register = 0x0D,
	DMA0_Master_Clear = 0x0D,
	DMA0_Clear_Mask_Register = 0x0E,
	DMA0_Write_Mask_Register = 0x0F
};

enum DMA1_Regiters {
	DMA1_Status_Register = 0xD0,
	DMA1_Command_Register = 0xD0,
	DMA1_Request_Register = 0xD2,
	DMA1_Single_Mask_Register = 0xD4,
	DMA1_Mode_Register = 0xD6,
	DMA1_Clear_Byte_FlipFlop_Register = 0xD8,
	DMA1_Intermediate_Register = 0xDA,
	DMA1_Master_Clear = 0xDA,
	DMA1_Clear_Mask_Register = 0xDC,
	DMA1_Write_Mask_Register = 0xDE
};

enum DMA0_Channel_Registers {
	DMA0_Channel0_Address_Register = 0x0,
	DMA0_Channel0_Counter_Register = 0x1,
	DMA0_Channel1_Address_Register = 0x2,
	DMA0_Channel1_Counter_Register = 0x3,
	DMA0_Channel2_Address_Register = 0x4,
	DMA0_Channel2_Counter_Register = 0x5,
	DMA0_Channel3_Address_Register = 0x6,
	DMA0_Channel3_Counter_Register = 0x7
};

enum DMA1_Channel_Registers {
	DMA1_Channel0_Address_Register = 0xC0,
	DMA1_Channel0_Counter_Register = 0xC2,
	DMA1_Channel1_Address_Register = 0xC4,
	DMA1_Channel1_Counter_Register = 0xC6,
	DMA1_Channel2_Address_Register = 0xC8,
	DMA1_Channel2_Counter_Register = 0xCA,
	DMA1_Channel3_Address_Register = 0xCC,
	DMA1_Channel3_Counter_Register = 0xCE
};

enum DMA0_Page_Registers {
	DMA0_Diagostic_Port = 0x80,
	DMA0_Channel2_Addr = 0x81,
	DMA0_Channel3_Addr = 0x82,
	DMA0_Channel1_Addr = 0x83,
	DMA0_Extra1 = 0x84,
	DMA0_Extra2 = 0x85,
	DMA0_Extra3 = 0x86,
	DMA0_Channel7_Addr = 0x87,
	DMA0_Channel6_Addr = 0x88,
	DMA0_Channel5_Addr = 0x89,
	DMA0_Extra4 = 0x8C,
	DMA0_Extra5 = 0x8D,
	DMA0_Extra6 = 0x8E,
	DMA0_Refresh = 0x8F
};

enum DMA_Mode_Register_Masks {
	DMA_Channel_Select_Mask = 0x3,

	DMA_Transfer_Type_Mask = 0xC,
	DMA_Controller_Self_Test = 0x0,
	DMA_Write_Transfer_Mask = 0x4,
	DMA_Read_Transfer_Mask = 0x8,
	DMA_Ivalid_Mask = 0xC,

	DMA_Auto_Reinitialize_Mask = 0x10,
	DMA_IDEC_Mask = 0x20,

	DMA_Mode_Mask = 0xC0,
	DMA_Transfer_On_Demand_Mask = 0x0,
	DMA_Single_Transfer_Mask = 0x40,
	DMA_Block_Transfer_Mask = 0x80,
	DMA_Cascade_Mode_Mask = 0xC0
};