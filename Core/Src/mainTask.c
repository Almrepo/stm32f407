/*
 * mainTask.c
 *
 *  Created on: Dec 26, 2019
 *      Author: alm
 */

#include "mainTask.h"
ow_t ow;
ow_rom_t rom_ids[20];
uint8_t  rom_found;


void init_sensor_temp_OW(void)
{


	   ow_init(&ow, NULL);
	     if (scan_onewire_devices(&ow, rom_ids, OW_ARRAYSIZE(rom_ids), &rom_found) == owOK) {
	               //printf("Devices scanned, found %d devices!\r\n", (int)rom_found);
	            } else {
	               // printf("Device scan error\r\n");
	            }
	            if (rom_found == 0) {
	            	//
	            	//todo

	            }
	}




void Get_tem_sensor_OW( float* m_temp)
{
  /* USER CODE BEGIN CallbackTimer01 */
  //enum sensor {T_sound,t_power};
   // struct_temp *qstruct;
   // qstruct =(struct_temp*) osMailAlloc(mail, osWaitForever);

  ow_ds18x20_start(&ow, NULL);
        if (rom_found) {

                    for (size_t i = 0; i < rom_found; i++) {

                       if (ow_ds18x20_is_b(&ow, &rom_ids[i])) {
                           float temp;
                        uint8_t resolution = ow_ds18x20_get_resolution(&ow, &rom_ids[i]);
                           if (ow_ds18x20_read(&ow, &rom_ids[i], &temp)) {
                             //  printf("Sensor %02u temperature is %d.%d degrees (%u bits resolution)\r\n",
                             //      (unsigned)i, (int)temp, (int)((temp * 1000.0f) - (((int)temp) * 1000)), (unsigned)resolution);

                              m_temp[i] =temp;


                             //  qstruct->temper[i]=temp;
                           }
                           }
                       }
                   }


        //temp_check((uint8_t)tp[t_power],(uint8_t)tp[T_sound]);
       // osMailPut(mail, qstruct);

		 return (uint8_t)rom_found;

  /* USER CODE END CallbackTimer01 */
}
