# 1 "C:\\Users\\xieyu\\AppData\\Roaming\\ZhRobot2.0.2\\Local Store\\scratchTemp\\project_Robot_competition4_5\\project_Robot_competition4_5.ino"
# 1 "C:\\Users\\xieyu\\AppData\\Roaming\\ZhRobot2.0.2\\Local Store\\scratchTemp\\project_Robot_competition4_5\\project_Robot_competition4_5.ino"
# 2 "C:\\Users\\xieyu\\AppData\\Roaming\\ZhRobot2.0.2\\Local Store\\scratchTemp\\project_Robot_competition4_5\\project_Robot_competition4_5.ino" 2
# 3 "C:\\Users\\xieyu\\AppData\\Roaming\\ZhRobot2.0.2\\Local Store\\scratchTemp\\project_Robot_competition4_5\\project_Robot_competition4_5.ino" 2


void __func__25968_25454_21021_22987_21270();
double _var_20809_30005_38408_20540;
void __func__22522_30784_24033_32447_36895_24230(double _var_36895_24230);
double _var_20809_30005_50;
double _var_20809_30005_49;
double _var_20809_30005_51;
double jilv;
void __func__36208_36317_31163(double sudu, double _var_32534_30721_36317_31163);
void __func__27979_35797_20809_30005();
double _var_20809_30005_52;
double _var_20809_30005_53;
void __func__35835_21462_20809_30005_20256_24863_22120_29366_24577();
void __func__36895_24230_25511_21046(double zmd, double ymd);
void __func__36935_32447_20572_27490(double _var_36895_24230, double light);
void __func__33258_23450_20041_36716_24367(double number1, double number2, double number3);
void __func__24033_32447_36895_24230(double xxsd, double bmjl);
void end();
double _var_24033_32447_36895_24230;
double _var_52_118_30005_26426_24320_21551_35282_24230;
double _var_52_118_30005_26426_20851_38381_35282_24230;
void got();


void __func__25968_25454_21021_22987_21270()
{
    _var_20809_30005_38408_20540 = 100;

}

void __func__22522_30784_24033_32447_36895_24230(double _var_36895_24230)
{
    __func__35835_21462_20809_30005_20256_24863_22120_29366_24577();

    if(((_var_20809_30005_50)==(1))){
        if(((_var_20809_30005_49)==(1))){
            if(((_var_20809_30005_51)==(1))){
                SetMotor(1,_var_36895_24230);
                SetMotor(2,_var_36895_24230);
            }else{
                SetMotor(1,(_var_36895_24230) * (0.2));
                SetMotor(2,_var_36895_24230);
            }
        }else{
            if(((_var_20809_30005_51)==(1))){
                SetMotor(1,_var_36895_24230);
                SetMotor(2,(_var_36895_24230) * (0.2));
            }else{
                SetMotor(1,_var_36895_24230);
                SetMotor(2,_var_36895_24230);
            }
        }
    }else{
        if(((_var_20809_30005_51)==(1))){
            if(((_var_20809_30005_49)==(1))){
                SetMotor(1,_var_36895_24230);
                SetMotor(2,_var_36895_24230);
            }else{
                SetMotor(1,_var_36895_24230);
                SetMotor(2,(_var_36895_24230) * (0.2));
                jilv = 3;
            }
        }else{
            if(((_var_20809_30005_49)==(1))){
                if(((_var_20809_30005_51)==(1))){
                    SetMotor(1,_var_36895_24230);
                    SetMotor(2,_var_36895_24230);
                }else{
                    SetMotor(1,(_var_36895_24230) * (0.2));
                    SetMotor(2,_var_36895_24230);
                    jilv = 1;
                }
            }else{
                if(((jilv)==(1))){
                    SetMotor(1,(0) - (_var_36895_24230));
                    SetMotor(2,_var_36895_24230);
                }else{
                    if(((jilv)==(3))){
                        SetMotor(1,_var_36895_24230);
                        SetMotor(2,(0) - (_var_36895_24230));
                    }else{
                        SetMotor(1,_var_36895_24230);
                        SetMotor(2,_var_36895_24230);
                    }
                }
            }
        }
    }

}

void __func__36208_36317_31163(double sudu, double _var_32534_30721_36317_31163)
{
    jilv = 0;

    SetEncoder(1);

    while(!((((GetEncoder(1))>0?(GetEncoder(1)):-(GetEncoder(1)))) > (_var_32534_30721_36317_31163)))
    {
        SetMotor(1,sudu);
        SetMotor(2,sudu);
    }

    SetMotor(1,0);

    SetMotor(2,0);

    SetBuzzer(9,523,125);

}

void __func__27979_35797_20809_30005()
{
    __func__35835_21462_20809_30005_20256_24863_22120_29366_24577();

    Set7SegDisplayChar(45,1,_var_20809_30005_49);

    Set7SegDisplayChar(45,2,_var_20809_30005_50);

    Set7SegDisplayDot(45,_var_20809_30005_51);

    Set7SegDisplayChar(45,3,_var_20809_30005_52);

    Set7SegDisplayChar(45,4,_var_20809_30005_53);

}

void __func__35835_21462_20809_30005_20256_24863_22120_29366_24577()
{
    _var_20809_30005_38408_20540 = 150;

    if((GetAdc(1)) < (_var_20809_30005_38408_20540)){
        _var_20809_30005_49 = 1;
    }else{
        _var_20809_30005_49 = 0;
    }

    if((GetAdc(2)) < (_var_20809_30005_38408_20540)){
        _var_20809_30005_50 = 1;
    }else{
        _var_20809_30005_50 = 0;
    }

    if((GetAdc(3)) < (_var_20809_30005_38408_20540)){
        _var_20809_30005_51 = 1;
    }else{
        _var_20809_30005_51 = 0;
    }

}

void __func__36895_24230_25511_21046(double zmd, double ymd)
{
    SetMotor(1,zmd);

    SetMotor(2,ymd);

    SetBuzzer(9,523,125);

}

void __func__36935_32447_20572_27490(double _var_36895_24230, double light)
{
    __func__35835_21462_20809_30005_20256_24863_22120_29366_24577();

    if(((light)==(1))){
        while(!((((_var_20809_30005_49)==(1))) && (((_var_20809_30005_50)==(1)))))
        {
            __func__22522_30784_24033_32447_36895_24230(_var_36895_24230);
        }
    }else{
        if(((light)==(3))){
            while(!((((_var_20809_30005_51)==(1))) && (((_var_20809_30005_50)==(1)))))
            {
                __func__22522_30784_24033_32447_36895_24230(_var_36895_24230);
            }
        }else{
            if(((light)==(13))){
                while(!((((_var_20809_30005_49)==(1))) && (((_var_20809_30005_51)==(1)))))
                {
                    __func__22522_30784_24033_32447_36895_24230(_var_36895_24230);
                }
            }else{
                if(((light)==(31))){
                    while(!((((_var_20809_30005_49)==(1))) || (((_var_20809_30005_51)==(1)))))
                    {
                        __func__22522_30784_24033_32447_36895_24230(_var_36895_24230);
                    }
                }else{
                }
            }
        }
    }

    __func__36208_36317_31163(30,200);

    SetBuzzer(9,523,125);

}

void __func__33258_23450_20041_36716_24367(double number1, double number2, double number3)
{
    SetMotor(1,number1);

    SetMotor(2,number2);

    _delay(0.2);

    __func__35835_21462_20809_30005_20256_24863_22120_29366_24577();

    if(((number3)==(1))){
        while(!(((_var_20809_30005_49)==(1))))
        {
            __func__35835_21462_20809_30005_20256_24863_22120_29366_24577();
        }
    }else{
        if(((number3)==(2))){
            while(!(((_var_20809_30005_50)==(1))))
            {
                __func__35835_21462_20809_30005_20256_24863_22120_29366_24577();
            }
        }else{
            if(((number3)==(3))){
                while(!(((_var_20809_30005_51)==(1))))
                {
                    __func__35835_21462_20809_30005_20256_24863_22120_29366_24577();
                }
            }else{
            }
        }
    }

    SetMotor(1,0);

    SetMotor(2,0);

    SetBuzzer(9,523,125);

}

void __func__24033_32447_36895_24230(double xxsd, double bmjl)
{
    SetEncoder(1);

    while(!((((GetEncoder(1))>0?(GetEncoder(1)):-(GetEncoder(1)))) > (bmjl)))
    {
        __func__22522_30784_24033_32447_36895_24230(xxsd);
    }

    SetMotor(1,0);

    SetMotor(2,0);

    SetBuzzer(9,523,125);

}

void end()
{
    __func__36208_36317_31163(30,30);

    __func__33258_23450_20041_36716_24367(0,25,2);

    __func__36935_32447_20572_27490(_var_24033_32447_36895_24230,3);

    __func__36208_36317_31163(30,30);

    __func__36935_32447_20572_27490(_var_24033_32447_36895_24230,3);

    __func__36208_36317_31163(30,30);

    __func__33258_23450_20041_36716_24367(25,0,2);

    SetServo(10,_var_52_118_30005_26426_24320_21551_35282_24230);

    __func__36935_32447_20572_27490(_var_24033_32447_36895_24230,13);

    __func__36208_36317_31163(30,30);

    __func__36208_36317_31163(-30,1000);

    SetServo(10,_var_52_118_30005_26426_20851_38381_35282_24230);

}

void got()
{
    __func__36208_36317_31163(-30,1000);

    __func__33258_23450_20041_36716_24367(25,0,2);

    __func__36935_32447_20572_27490(_var_24033_32447_36895_24230,13);

    __func__36208_36317_31163(30,30);

    __func__33258_23450_20041_36716_24367(0,25,2);

    __func__36935_32447_20572_27490(_var_24033_32447_36895_24230,3);

    __func__33258_23450_20041_36716_24367(0,25,2);

    __func__36935_32447_20572_27490(_var_24033_32447_36895_24230,3);

    __func__33258_23450_20041_36716_24367(0,25,2);

    __func__36935_32447_20572_27490(_var_24033_32447_36895_24230,13);

}


void setup(){
    _var_52_118_30005_26426_24320_21551_35282_24230 = 0;
    _var_52_118_30005_26426_20851_38381_35282_24230 = 135;
    _var_24033_32447_36895_24230 = 30;
    got();

}

void loop(){




}
