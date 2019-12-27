#include "GY86.h"
//��ΪMPU9150����Ĵ�����AK8975�����⣬���Ժ����MPU9150ΪGY86�ˣ�����Ĵ�����HMC5883L���ܽϺá�
//����������л�ͨ��AK8975_EN����������Ϊ�˱����鷳�����ļ�������ͳһ��MPU9150

uint16_t Cal_C[7]; //���ڴ��PROM�е�6������
uint32_t D1_Pres, D2_Temp; // �������ѹ�����¶�
extern float pressure; //�¶Ȳ�������ѹ
extern float Temperature; //ʵ���¶�
float dT, Temperature2; //ʵ�ʺͲο��¶�֮��Ĳ���,�м�ֵ
double OFF, SENS; //ʵ���¶ȵ���,ʵ���¶�������
float Aux, OFF2, SENS2; //�¶�У��ֵ
uint32_t ex_Pressure; //���ڶ���ת��ֵ
uint8_t exchange_num[8];

/**************************************
 * ��������MPU6050_Init
 * ����  ����ʼ��Mpu6050
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 *************************************/
int MPU6050_Init(void)
{
    if (I2C_ReadByte(MPU6050_Addr, MPU6050_WHO_AM_I) != MPU6050_Device_ID) { //���MPU6050�Ƿ�����
        return 0;
    }
    I2C_WriteByte(MPU6050_Addr, MPU6050_PWR_MGMT_1, 0x00); //�������״̬,ʹ���ڲ�8MHz����
    I2C_WriteByte(MPU6050_Addr, MPU6050_SMPLRT_DIV, 0x00); //������Ƶ (����Ƶ�� = ���������Ƶ�� / (1+DIV)������Ƶ��1000hz��
    I2C_WriteByte(MPU6050_Addr, MPU6050_CONFIG, 0x06); //���õ�ͨ�˲�
    I2C_WriteByte(MPU6050_Addr, MPU6050_GYRO_CONFIG, 0x18); //������������+-2000��/�� (��ͷֱ��� = 2^15/2000 = 16.4LSB/��/��
    I2C_WriteByte(MPU6050_Addr, MPU6050_ACCEL_CONFIG, 0x08); //���ٶ�������+-4g   (��ͷֱ��� = 2^15/4g = 8192LSB/g )
    I2C_WriteByte(MPU6050_Addr, MPU6050_INT_PIN_CFG, 0x02); //����·ģʽ
    I2C_WriteByte(MPU6050_Addr, MPU6050_USER_CTRL, 0x00); //�ر���ģʽ

    return 1;
}

/**************************************
 * HMC5883L_Init
 * ����  ����ʼ��HMC5883L
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 *************************************/
void HMC5883L_Init(void)
{
    // ���ñ�׼�����������75HZ
    I2C_WriteByte(HMC5883L_Addr, HMC5883L_CONFIG_A, 0x18);
    // ���ô������ų���Χ��1.3Ga
    I2C_WriteByte(HMC5883L_Addr, HMC5883L_CONFIG_B, 0x20);
    // ��continuous measurementģʽ
    I2C_WriteByte(HMC5883L_Addr, HMC5883L_MODE, 0x00);
}

/************************************************************   
* ������:MS561101BA_Init   
* ���� : MS561101BA��ʼ��
* ����  :��   
* ���  :��    
*/
void MS561101BA_Init(void)
{
    MS561101BA_Reset();
    delay_ms(100);
    MS561101BA_ReadPROM();
    delay_ms(100);
}

/*************************************
 * ��������GetData_MPU6050
 * ����  �����16λ����
 * ����  ��REG_Address �Ĵ�����ַ
 * ���  �����ؼĴ�������
 * ����  ���ⲿ����
 ************************************/
uint16_t GetData_MPU6050(uint8_t REG_Address)
{ //����ʹ�ã���Ϊ�������������ݸ���
    uint8_t H, L;
    H = I2C_ReadByte(MPU6050_Addr, REG_Address);
    L = I2C_ReadByte(MPU6050_Addr, REG_Address + 1);
    return (H << 8) | L; //�ϳ�����
}

/*************************************
 * ��������GetData_HMC5883L
 * ����  �����16λ����
 * ����  ��REG_Address �Ĵ�����ַ
 * ���  �����ؼĴ�������
 * ����  ���ⲿ����
 ************************************/
uint16_t GetData_HMC5883L(uint8_t REG_Address)
{ //����ʹ�ã���Ϊ�������������ݸ���
    uint8_t H, L;
    H = I2C_ReadByte(HMC5883L_Addr, REG_Address);
    L = I2C_ReadByte(HMC5883L_Addr, REG_Address + 1);
    return (H << 8) | L; //�ϳ�����
}

/************************************************************   
* ������:MS561101BA_Reset   
* ���� : ��λ  
* ����  :��   
* ���  :��    
*/
void MS561101BA_Reset(void)
{
    I2C_NoAddr_WriteByte(MS561101BA_Addr, MS561101BA_RESET);
}

/************************************************************   
* ������:MS561101BA_readPROM   
* ���� : ��PROM��ȡ����У׼����
* ����  :��   
* ���  :��    
*/
void MS561101BA_ReadPROM(void)
{
    u8 i;
    for (i = 0; i <= MS561101BA_PROM_REG_COUNT; i++) {
        Cal_C[i] = I2C_Read_2Bytes(MS561101BA_Addr, MS561101BA_PROM_BASE_ADDR + (i * MS561101BA_PROM_REG_SIZE));
    }
}

/************************************************************   
* ������:MS561101BA_DO_CONVERSION   
* ���� :  
* ����  :��   
* ���  :��    
*/
uint32_t MS561101BA_Do_Conversion(uint8_t command)
{
    uint32_t conversion;

    I2C_NoAddr_WriteByte(MS561101BA_Addr, command);

    delay_ms(9);

    conversion = I2C_Read_3Bytes(MS561101BA_Addr, 0);

    return conversion;
}

/************************************************************   
* ������:MS561101BA_GetTemperature   
* ���� : ��ȡ�����¶�
* ����  :��������   
* ���  :��    
*/
void MS561101BA_GetTemperature(u8 OSR_Temp)
{

    D2_Temp = MS561101BA_Do_Conversion(OSR_Temp);

    delay_ms(9);

    dT = D2_Temp - (((uint32_t)Cal_C[5]) << 8);
    Temperature = 2000 + dT * ((uint32_t)Cal_C[6]) / 0x800000; //����¶�ֵ��100����2001��ʾ20.01��
}

/************************************************************   
* ������:MS561101BA_GetPressure   
* ���� : ��ȡ������ѹ
* ����  :��������   
* ���  :��    
*/
void MS561101BA_GetPressure(u8 OSR_Pres)
{

    D1_Pres = MS561101BA_Do_Conversion(OSR_Pres);

    delay_ms(9);

    OFF = (uint32_t)(Cal_C[2] << 16) + ((uint32_t)Cal_C[4] * dT) / 0x80;
    SENS = (uint32_t)(Cal_C[1] << 15) + ((uint32_t)Cal_C[3] * dT) / 0x100;
    //�¶Ȳ���
    if (Temperature < 2000) // second order temperature compensation when under 20 degrees C
    {
        Temperature2 = (dT * dT) / 0x80000000;
        Aux = (Temperature - 2000) * (Temperature - 2000);
        OFF2 = 2.5f * Aux;
        SENS2 = 1.25f * Aux;
        if (Temperature < -1500) {
            Aux = (Temperature + 1500) * (Temperature + 1500);
            OFF2 = OFF2 + 7 * Aux;
            SENS2 = SENS + 5.5f * Aux;
        }
    } else //(Temperature > 2000)
    {
        Temperature2 = 0;
        OFF2 = 0;
        SENS2 = 0;
    }

    Temperature = Temperature - Temperature2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;

    pressure = (D1_Pres * SENS / 0x200000 - OFF) / 0x8000;
}
