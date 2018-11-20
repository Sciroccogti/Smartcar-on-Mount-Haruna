
#include "KEA128_i2c.h"

unsigned char MasterTransmission;
unsigned char SlaveID;

I2C_Type * I2CN[2] = I2C_BASES; //��������ָ�����鱣�� I2CN �ĵ�ַ


//�ȴ� I2C_S  ������ֿ��������
void i2c_Wait(I2Cn_e i2cn)
{
    uint16 num;
    uint16 wait_num=500;//�ȴ������������趨�Ĵ������ر�Ӳ��IIC��ʹ��ģ��IIC����stop�źţ�Ȼ��������Ӳ��IIC��
                        //�����500��ͨ������Ϊ100K��ʱ���������ʹ�ã����ͨ�����ʽϵͿ����ʵ��������ֵ��
                        //�������Կ�����ͨ�ų����ʱ���Զ��ָ���
    while(( I2CN[i2cn]->S1 & I2C_S_IICIF_MASK)==0)
    {
        num++;
        if(num>wait_num) 
        {
            I2CN[i2cn]->C1 = 0;
            if(i2cn == i2c0)    SIM->SCGC &= ~(uint32)SIM_SCGC_I2C0_MASK;         
            else                SIM->SCGC &= ~(uint32)SIM_SCGC_I2C1_MASK;           
            IIC_init();
            IIC_stop();
            if(i2cn == i2c0)    SIM->SCGC |= SIM_SCGC_I2C0_MASK;         
            else                SIM->SCGC |= SIM_SCGC_I2C1_MASK;          
            I2CN[i2cn]->C1 = I2C_C1_IICEN_MASK;
            break;
        }
    }
    I2CN[i2cn]->S1 |= I2C_S_IICIF_MASK ;
}

//�����ź�
#define i2c_Start(NUM)             {I2CN[NUM]->C1 |= (I2C_C1_TX_MASK | I2C_C1_MST_MASK);}    //MST ��0��1��������ʼ�źţ�TX = 1 ���뷢��ģʽ

//ֹͣ�ź�
#define i2c_Stop(NUM)              {I2CN[NUM]->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK);}   //MST ��1��0������ֹͣ�źţ�TX = 0 �������ģʽ

//�ظ�����
#define i2c_RepeatedStart(NUM)     {I2CN[NUM]->C1 |= I2C_C1_RSTA_MASK;}

//�������ģʽ(Ӧ��,��Ҫ���ն�����ݣ��������һ���ֽ�ǰ��Ҫ����Ӧ��i2c_DisableAck)
#define i2c_EnterRxMode(NUM)       {I2CN[NUM]->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);}  //

//�������ģʽ(��Ӧ��,ֻ����һ���ֽ�)
#define i2c_PutinRxMode(NUM)       {I2CN[NUM]->C1 &= ~I2C_C1_TX_MASK; I2CN[NUM]->C1 |= I2C_C1_TXAK_MASK;}

//����Ӧ��(�������һ���ֽ�)
#define i2c_DisableAck(NUM)        {I2CN[NUM]->C1 |= I2C_C1_TXAK_MASK;}								

//дһ���ֽ�
#define i2c_write_byte(NUM,data)   {I2CN[NUM]->D = data; i2c_Wait(NUM);}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      I2C��ʼ��
//  @param      i2cn        I2Cģ��(i2c0,i2c1)
//  @param      baud        �����Ĳ�����
//  @return                 ʵ�ʵĲ�����
//  @return     void
//  @since      v2.0
//  Sample usage:           i2c_init(i2c0,400*1000);     // ��ʼ��i2c0�������Ĳ�����Ϊ400k
//-------------------------------------------------------------------------------------------------------------------
uint32 i2c_init(I2Cn_e i2cn, uint32 baud)
{
    //��� ICR ��Ӧ��  SCL_divider ���� ��KEA128RM.pdf����537ҳ��I2C ��Ƶ���ͱ���ֵ
    uint16 ICR_2_SCL_divider[0x40]  =
    {
        20, 22, 24, 26, 28, 30, 34, 40, 28, 32, 36, 40, 44, 48, 56, 68,
        48, 56, 64, 72, 80, 88, 104, 128, 80, 96, 112, 128, 144, 160, 192, 240,
        160, 192, 224, 256, 288, 320, 384, 480, 320, 384, 448, 512, 576, 640, 768, 960,
        640, 768, 896, 1024, 1152, 1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840
    };

    uint8 mult;
    
    uint16 scldiv =  0;  //��ѵķ�Ƶϵ��

    //��Ҫ�� ICR_2_SCL_divider ���ҵ� ����ѷ�Ƶϵ��scldiv������� ��Ƶϵ��
    uint8 icr, n;
    uint16 min_Dvalue = ~0, Dvalue;
    
    if(i2cn == i2c0)
    {
        // ����ʱ�� 
        SIM->SCGC |= SIM_SCGC_I2C0_MASK;           //���� I2C0ʱ��
        //��������
        if(A3 == I2C0_SCL_PIN)
        {
            SIM->PINSEL &= ~(uint32)SIM_PINSEL_I2C0PS_MASK;
        }
        else
        {
            SIM->PINSEL |= SIM_PINSEL_I2C0PS_MASK;
        }
        
    }
    else
    {
        // ����ʱ�� 
        SIM->SCGC |= SIM_SCGC_I2C1_MASK;           //���� I2C1ʱ��
        //��������
        if(E1 == I2C1_SCL_PIN)
        {
            SIM->PINSEL1 &= ~(uint32)SIM_PINSEL1_I2C1PS_MASK;
        }
        else
        {
            SIM->PINSEL1 |= SIM_PINSEL1_I2C1PS_MASK;
        }
    }

    // ����Ƶ��

    // I2C baud rate = bus speed (Hz)/(mul �� SCL divider)
    // SDA hold time = bus period (s) �� mul �� SDA hold value
    // SCL start hold time = bus period (s) �� mul �� SCL start hold value
    // SCL stop hold time = bus period (s) �� mul �� SCL stop hold value

    
    mult = bus_clk_khz*1000/baud/3840;

    scldiv =  bus_clk_khz * 1000 / ( (1<<mult) * baud );    //��ѵķ�Ƶϵ��
    
    n = 0x40;
    while(n)                                                //ѭ�������ɨ�裬�ҳ���ӽ��� ��Ƶϵ��
    {
        n--;
        Dvalue = abs(scldiv - ICR_2_SCL_divider[n]);
        if(Dvalue == 0)
        {
            icr = n;
            break;                                          //�˳�whileѭ��
        }

        if(Dvalue < min_Dvalue)
        {
            icr = n;
            min_Dvalue = Dvalue;
        }
    }

    I2CN[i2cn]->F  = I2C_F_MULT(mult) | I2C_F_ICR(icr);     // I2C��Ƶ�Ĵ���

    // ʹ�� I2C 
    I2CN[i2cn]->C1 = ( 0
                       | I2C_C1_IICEN_MASK       //ʹ��I2C
                       //| I2C_C1_IICIE_MASK       //ʹ���ж�
                     );

    return (  bus_clk_khz * 1000 / ( (1<<mult) * ICR_2_SCL_divider[icr])  );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      I2Cͨ�Ž�������Ҫ���õĺ�������
//  @return     void
//  @since      v2.0
//  @note		���ͨ��ʧ�ܣ��ɳ����������ʱֵ��ȷ���Ƿ���ʱ���µ�
//-------------------------------------------------------------------------------------------------------------------
void i2c_delay(void)
{
    volatile uint16 n = 10;     //ע�⣬�������̫С���ᵼ�¶�ȡ����

    while(n--);
}    

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡI2C�豸ָ����ַ�Ĵ���������
//  @param      i2cn        I2Cģ��(i2c0��i2c1)
//  @param      SlaveID     �ӻ���ַ(7λ��ַ)
//  @param      reg         �ӻ��Ĵ�����ַ
//  @return                 ��ȡ�ļĴ���ֵ
//  @since      v2.0
//  Sample usage:       	uint8 value = i2c_read_reg(i2c0, 0x2D, 0x50);//��ȡ0x50��ַ�����ݣ��ӻ���ַΪ0x2D
//-------------------------------------------------------------------------------------------------------------------
uint8 i2c_read_reg(I2Cn_e i2cn, uint8 SlaveID, uint8 reg)
{

    //��д��Ĵ�����ַ,�ٶ�ȡ����,��˴˹����� I2C �ĸ��ϸ�ʽ,�ı����ݷ���ʱ��Ҫ��������
    //��ַ�ǵ���λ
    uint8 result;

    i2c_Start(i2cn);                                    //���������ź�

    i2c_write_byte(i2cn, (SlaveID << 1) | MWSR);        //���ʹӻ���ַ��дλ

    i2c_write_byte(i2cn, reg);                          //���ʹӻ���ļĴ�����ַ

    i2c_RepeatedStart(i2cn);                            //���ϸ�ʽ���������������ź�

    i2c_write_byte(i2cn, ( SlaveID << 1) | MRSW );      //���ʹӻ���ַ�Ͷ�λ

    i2c_PutinRxMode(i2cn);                              //�������ģʽ(��Ӧ��,ֻ����һ���ֽ�)
    result = I2CN[i2cn]->D;                             //��ٶ�ȡһ�Σ�������������
    i2c_Wait(i2cn);                                     //�ȴ��������

    i2c_Stop(i2cn);                                     //����ֹͣ�ź�

    result = I2CN[i2cn]->D;                             //��ȡ����

    i2c_delay();                                        //������ʱһ�£��������

    return result;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡI2C�豸ָ����ַ�Ĵ���������
//  @param      i2cn        I2Cģ��(i2c0��i2c1)
//  @param      SlaveID     �ӻ���ַ(7λ��ַ)
//  @param      reg         �ӻ��Ĵ�����ַ
//  @param      num         ��ȡ�ֽ���
//  @param      addr        ��ȡ�����ݴ洢�ĵ�ַ
//  @return     void
//  @since      v2.0
//  Sample usage:       	uint8 value = i2c_read_reg(i2c0, 0x2D, 0x50, 10, buf);//��ȡ0x50��ַ�����ݣ��ӻ���ַΪ0x2D��ʼ��10���ֽ�
//-------------------------------------------------------------------------------------------------------------------

uint8 i2c_read_reg_bytes(I2Cn_e i2cn, uint8 SlaveID, uint8 reg, uint8 num, uint8 * addr)
{
    //��д��Ĵ�����ַ,�ٶ�ȡ����,��˴˹����� I2C �ĸ��ϸ�ʽ,�ı����ݷ���ʱ��Ҫ��������
    //��ַ�ǵ���λ
    uint8 result;
    
    i2c_Start(i2cn);                                    //���������ź�

    i2c_write_byte(i2cn, (SlaveID << 1) | MWSR);        //���ʹӻ���ַ��дλ

    i2c_write_byte(i2cn, reg);                          //���ʹӻ���ļĴ�����ַ

    i2c_RepeatedStart(i2cn);                            //���ϸ�ʽ���������������ź�

    i2c_write_byte(i2cn, ( SlaveID << 1) | MRSW );      //���ʹӻ���ַ�Ͷ�λ

    i2c_EnterRxMode(i2cn);                              //�������ģʽ(��Ӧ��,ֻ����һ���ֽ�)
    
    while(--num)
    {
        result = I2CN[i2cn]->D;                         //��ٶ�ȡһ�Σ�������������
        i2c_Wait(i2cn);                                 //�ȴ��������
        *addr = I2CN[i2cn]->D;                         //��ȡ����
        i2c_delay();                                    //������ʱһ�£��������
        addr++;
    }
    i2c_DisableAck(i2cn);
    i2c_PutinRxMode(i2cn);
    result = I2CN[i2cn]->D;                             //��ٶ�ȡһ�Σ�������������
    i2c_Wait(i2cn);                                     //�ȴ��������
    i2c_Stop(i2cn);                                     //����ֹͣ�ź�
    *addr = I2CN[i2cn]->D;                              //��ȡ����

    i2c_delay();                                        //������ʱһ�£��������
    
    return result;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      д��һ���ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
//  @param      i2cn        I2Cģ��(i2c0��i2c1)
//  @param      SlaveID     �ӻ���ַ(7λ��ַ)
//  @param      reg         �ӻ��Ĵ�����ַ
//  @param      Data        ����
//  @return     void
//  @since      v2.0
//  Sample usage:       	i2c_write_reg(i2c0, 0x2D, 0x50,2);     //д������2��0x50��ַ���ӻ���ַΪ0x2D
//-------------------------------------------------------------------------------------------------------------------
void i2c_write_reg(I2Cn_e i2cn, uint8 SlaveID, uint8 reg, uint8 Data)
{

    i2c_Start(i2cn);                                    //���������ź�

    i2c_write_byte(i2cn, ( SlaveID << 1 ) | MWSR);      //���ʹӻ���ַ��дλ

    i2c_write_byte(i2cn, reg);                          //���ʹӻ���ļĴ�����ַ

    i2c_write_byte(i2cn, Data);                         //������Ҫд�������

    i2c_Stop(i2cn);

    i2c_delay();                                        //��ʱ̫�̵Ļ�������д����
}
