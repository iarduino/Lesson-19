// Библиотека «iarduino_RTC» разработана для RTC модуля http://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-rtc-trema-modul.html
// Вывод времени осуществляется на LCD 1602 I2C дисплей http://iarduino.ru/shop/Displei/simvolnyy-displey-golubaya-podsvetka-lcd1602-iic-i2c.html

// Подключаем библиотеки:
#include <Wire.h>                            // подключаем библиотеку для работы с шиной I2C
#include <LiquidCrystal_I2C.h>               // подключаем библиотеку для работы с LCD дисплеем
#include <iarduino_RTC.h>                    // подключаем библиотеку для работы с RTC модулем
LiquidCrystal_I2C lcd(0x27,16,2);            // объявляем  переменную для работы с LCD дисплеем, указывая параметры дисплея (адрес I2C = 0x27, количество столбцов = 16, количество строк = 2)
iarduino_RTC time(RTC_DS1307);               // объявляем  переменную для работы с RTC модулем,  указывая название модуля
// Объявляем переменные и константы:
const uint8_t PIN_button_SET  = 2;           // указываем номер вывода arduino, к которому подключена кнопка SET
const uint8_t PIN_button_UP   = 3;           // указываем номер вывода arduino, к которому подключена кнопка UP
const uint8_t PIN_button_DOWN = 4;           // указываем номер вывода arduino, к которому подключена кнопка DOWN
const uint8_t PIN_alarm_TONE  = 5;           // указываем номер вывода arduino, к которому подключён зуммер будильника
const uint8_t PIN_alarm_LED   = 13;          // указываем номер вывода arduino, к которому подключён светодиод
      uint8_t VAR_mode_SHOW   = 1;           // режим вывода: 1-время 2-дата 3-время_будильника
      uint8_t VAR_mode_SET    = 0;           // режим установки времени: 0-нет 1-сек 2-мин 3-час 4-день 5-мес 6-год 7-день_недели 8-мин_будильника 9-час_будильника
      uint8_t VAR_alarm_MIN   = 0;           // будильник минуты
      uint8_t VAR_alarm_HOUR  = 0;           // будильник часы
      bool    VAR_alarm_FLAG1 = true;        // будильник разрешение работы
      bool    VAR_alarm_FLAG2 = false;       // будильник совершение действий (сигнализация)
      byte    MAS_alarm_SYM[8]={B00000,B01110,B10101,B10111,B10001,B01110,B00000,B00000};  // символ будильника для отображения на дисплее
void setup() {
  pinMode(PIN_button_SET,  INPUT);           // устанавливаем режим работы вывода PIN_button_SET,  как "вход"
  pinMode(PIN_button_UP,   INPUT);           // устанавливаем режим работы вывода PIN_button_UP,   как "вход"
  pinMode(PIN_button_DOWN, INPUT);           // устанавливаем режим работы вывода PIN_button_DOWN, как "вход"
  pinMode(PIN_alarm_TONE, OUTPUT);           // устанавливаем режим работы вывода PIN_alarm_TONE,  как "выход"
  pinMode(PIN_alarm_LED,  OUTPUT);           // устанавливаем режим работы вывода PIN_alarm_LED,   как "выход"
  digitalWrite(PIN_alarm_TONE, LOW);         // устанавливаем уровень логического «0» на выводе PIN_alarm_TONE
  digitalWrite(PIN_alarm_LED,  LOW);         // устанавливаем уровень логического «0» на выводе PIN_alarm_LED
  delay(300);                                // ждем 300мс
  time.begin();                              // инициируем RTC модуль
  lcd.init();                                // инициируем LCD дисплей
  lcd.backlight();                           // включаем подсветку LCD дисплея
  lcd.createChar(1, MAS_alarm_SYM);          // загружаем символ будильника под номером 1
}
void loop(){
  if(millis()%1000==0){                      // если прошла 1 секунда
    if(VAR_mode_SET==0){                     // если дата/время/будильник выводятся, а не устанавливаются
      lcd.setCursor(0, 0);                   // устанавливаем курсор в позицию: столбец 0, строка 0
      lcd.print("iArduino.ru");              // выводим текст "iArduino.ru"
      lcd.setCursor(15, 0);                  // устанавливаем курсор в позицию: столбец 15, строка 0
      lcd.print(VAR_alarm_FLAG1?"\1":" ");   // выводим значёк будильника
    }else{                                   // если дата/время/будильник устанавливаются, а не выводятся
      lcd.setCursor(0, 0);                   // устанавливаем курсор в позицию: столбец 0, строка 0
      if(VAR_mode_SHOW==1){lcd.print("Set time:" );}
      if(VAR_mode_SHOW==2){lcd.print("Set date:" );}
      if(VAR_mode_SHOW==3){lcd.print("Set alarm:");}
    }
    if(VAR_mode_SHOW==1){                    // если установлен режим вывода времени
      lcd.setCursor(0, 1);                   // устанавливаем курсор в позицию: столбец 0, строка 1
      lcd.print(time.gettime("H:i:s"));      // выводим время
    }
    if(VAR_mode_SHOW==2){                    // если установлен режим вывода даты
      lcd.setCursor(0, 1);                   // устанавливаем курсор в позицию: столбец 0, строка 1
      lcd.print(time.gettime("d-m-Y   D"));  // выводим дату
    }
    if(VAR_mode_SHOW==3){                    // если установлен режим вывода будильника
      lcd.setCursor(0, 1);                   // устанавливаем курсор в позицию: столбец 0, строка 1
      if(VAR_mode_SET==0){lcd.print("Alarm ");}
      time.gettime();                        // обновляем значение переменной time.seconds, для "мигания" устанавливаемым параметром будильника
      if(VAR_mode_SET==9&&time.seconds%2){lcd.print("  ");}else{if(VAR_alarm_HOUR<10){lcd.print("0");} lcd.print(VAR_alarm_HOUR);}
      lcd.print(":");
      if(VAR_mode_SET==8&&time.seconds%2){lcd.print("  ");}else{if(VAR_alarm_MIN<10){ lcd.print("0");} lcd.print(VAR_alarm_MIN);}
    }
    delay(1);                                // приостанавливаем на 1 мс, чтоб не выводить время несколько раз за 1мс
//  проверка будильника
    if(VAR_alarm_FLAG1){                     // если будильник включён
      if(time.seconds==00){                  // если в текущем времени 0 секунд
        if(time.minutes==VAR_alarm_MIN){     // если совпали минуты
          if(time.Hours==VAR_alarm_HOUR){    // если совпали часы
            VAR_alarm_FLAG2=true;            // устанавливаем флаг разрешающий совершение действий будильника (сигнализация)
          }
        }
      }
    }else{VAR_alarm_FLAG2=false;}            // если будильник выключен, то сбрасываем флаг разрешающий совершение действий будильника (сигнализация)
    if(VAR_alarm_FLAG2){Func_alarm_action();}// запускаем действия будильника
  }
  Func_buttons_control();                    // передаём управление кнопкам
}

// Функция управления кнопками:
void Func_buttons_control(){
  uint8_t i=0;
  time.blinktime(VAR_mode_SET);                                    // мигаем устанавливаемым параметром (если VAR_mode_SET больше 0)
//Если часы находятся в режиме установки даты/времени
  if(VAR_mode_SET){
//  Если нажата кнопка UP
    if(digitalRead(PIN_button_UP  )){
      while(digitalRead(PIN_button_UP)){delay(50);}                // ждём пока мы не отпустим кнопку UP
      if(VAR_alarm_FLAG2){VAR_alarm_FLAG2=false;}else{             // если будильник сработал, то выключаем его
        switch (VAR_mode_SET){                                     // инкремент (увеличение) устанавливаемого значения
        /* сек */ case 1: time.settime(0,                                   -1, -1, -1, -1, -1, -1); break;
        /* мин */ case 2: time.settime(-1, (time.minutes==59?0:time.minutes+1), -1, -1, -1, -1, -1); break;
        /* час */ case 3: time.settime(-1, -1, (time.Hours==23?0:time.Hours+1),     -1, -1, -1, -1); break;
        /* дни */ case 4: time.settime(-1, -1, -1, (time.day==31?1:time.day+1),         -1, -1, -1); break;
        /* мес */ case 5: time.settime(-1, -1, -1, -1, (time.month==12?1:time.month+1),     -1, -1); break;
        /* год */ case 6: time.settime(-1, -1, -1, -1, -1, (time.year==99?0:time.year+1),       -1); break;
        /* д.н.*/ case 7: time.settime(-1, -1, -1, -1, -1, -1, (time.weekday==6?0:time.weekday+1) ); break;
        /* м.б.*/ case 8: VAR_alarm_MIN =VAR_alarm_MIN ==59?0:VAR_alarm_MIN +1;                      break;
        /* ч.б.*/ case 9: VAR_alarm_HOUR=VAR_alarm_HOUR==23?0:VAR_alarm_HOUR+1;                      break;
        }
      }
    }
//  Если нажата кнопка DOWN
    if(digitalRead(PIN_button_DOWN)){
      while(digitalRead(PIN_button_DOWN)){delay(50);}              // ждём пока мы её не отпустим
      if(VAR_alarm_FLAG2){VAR_alarm_FLAG2=false;}else{             // если будильник сработал, то выключаем его
        switch (VAR_mode_SET){                                     // декремент (уменьшение) устанавливаемого значения
        /* сек */ case 1: time.settime(0,                                   -1, -1, -1, -1, -1, -1); break;
        /* мин */ case 2: time.settime(-1, (time.minutes==0?59:time.minutes-1), -1, -1, -1, -1, -1); break;
        /* час */ case 3: time.settime(-1, -1, (time.Hours==0?23:time.Hours-1),     -1, -1, -1, -1); break;
        /* дни */ case 4: time.settime(-1, -1, -1, (time.day==1?31:time.day-1),         -1, -1, -1); break;
        /* мес */ case 5: time.settime(-1, -1, -1, -1, (time.month==1?12:time.month-1),     -1, -1); break;
        /* год */ case 6: time.settime(-1, -1, -1, -1, -1, (time.year==0?99:time.year-1),       -1); break;
        /* д.н.*/ case 7: time.settime(-1, -1, -1, -1, -1, -1, (time.weekday==0?6:time.weekday-1) ); break;
        /* м.б.*/ case 8: VAR_alarm_MIN =VAR_alarm_MIN ==0?59:VAR_alarm_MIN -1;                      break;
        /* ч.б.*/ case 9: VAR_alarm_HOUR=VAR_alarm_HOUR==0?23:VAR_alarm_HOUR-1;                      break;
        }
      }
    }
//  Если нажата кнопка SET
    if(digitalRead(PIN_button_SET)){
      while(digitalRead(PIN_button_SET)){                          // ждём пока мы её не отпустим
        delay(10);
        if(i<200){i++;}else{lcd.clear();}                          // фиксируем, как долго удерживается кнопка SET, если дольше 2 секунд, то стираем экран
      }
      if(VAR_alarm_FLAG2){VAR_alarm_FLAG2=false;}else{             // если будильник сработал, то выключаем его
        if(i<200){                                                 // если кнопка SET удерживалась меньше 2 секунд
          VAR_mode_SET++;                                          // переходим к следующему устанавливаемому параметру
          if(VAR_mode_SHOW==1 && VAR_mode_SET>3){VAR_mode_SET=1;}  // возвращаемся к первому устанавливаемому параметру
          if(VAR_mode_SHOW==2 && VAR_mode_SET>7){VAR_mode_SET=4;}  // возвращаемся к первому устанавливаемому параметру
          if(VAR_mode_SHOW==3 && VAR_mode_SET>9){VAR_mode_SET=8;}  // возвращаемся к первому устанавливаемому параметру
        }else{                                                     // если кнопка SET удерживалась дольше 2 секунд, то требуется выйти из режима установки даты/времени
          VAR_mode_SET=0;                                          // выходим из режима установки даты/времени
        }
      }
    }
//Если часы находятся в режиме вывода даты/времени
  }else{
//  Если нажата кнопка UP
    if(digitalRead(PIN_button_UP  )){
      while(digitalRead(PIN_button_UP)){delay(50);}                // ждём пока мы её не отпустим
      if(VAR_alarm_FLAG2){VAR_alarm_FLAG2=false;}else{             // если будильник сработал, то выключаем его
        if(VAR_mode_SHOW==3){VAR_alarm_FLAG1=VAR_alarm_FLAG1?0:1;} // если установлен режим вывода будильника, то включаем/выключаем будильник
      }
    }
//  Если нажата кнопка DOWN
    if(digitalRead(PIN_button_DOWN)){
      while(digitalRead(PIN_button_DOWN)){delay(50);}              // ждём пока мы её не отпустим
      if(VAR_alarm_FLAG2){VAR_alarm_FLAG2=false;}else{             // если будильник сработал, то выключаем его
        if(VAR_mode_SHOW==3){VAR_alarm_FLAG1=VAR_alarm_FLAG1?0:1;} // если установлен режим вывода будильника, то включаем/выключаем будильник
      }
    }
//  Если нажата кнопка SET
    if(digitalRead(PIN_button_SET)){
      while(digitalRead(PIN_button_SET)){delay(10);                // ждём пока мы её не отпустим
        if(i<200){i++;}else{lcd.clear();}                          // фиксируем, как долго удерживается кнопка SET, если дольше 2 секунд, то стираем экран
      }
      if(VAR_alarm_FLAG2){VAR_alarm_FLAG2=false;}else{             // если будильник сработал, то выключаем его
        if(i<200){                                                 // если кнопка SET удерживалась меньше 2 секунд
          lcd.clear();                                             // стираем экран
          VAR_mode_SHOW++; if(VAR_mode_SHOW>3){VAR_mode_SHOW=1;}   // меняем режим вывода: дата/время/будильник
        }else{                                                     // если кнопка SET удерживалась дольше 2 секунд, то требуется перейти в режим установки даты/времени
          if(VAR_mode_SHOW==1){VAR_mode_SET=1;}                    // если установлен режим вывода времени,    то устанавливаемый параметр - секунды
          if(VAR_mode_SHOW==2){VAR_mode_SET=4;}                    // если установлен режим вывода даты,       то устанавливаемый параметр - день
          if(VAR_mode_SHOW==3){VAR_mode_SET=8;}                    // если установлен режим вывода будильника, то устанавливаемый параметр - минуты_будильника
        }
      }
    }
  }
}

// Функция выполняющая действия будильника:
void Func_alarm_action(){
    digitalWrite(PIN_alarm_LED, HIGH);
    tone(PIN_alarm_TONE, 2000); delay(100); noTone(PIN_alarm_TONE); delay(100);
    tone(PIN_alarm_TONE, 2000); delay(100); noTone(PIN_alarm_TONE); delay(100);
    tone(PIN_alarm_TONE, 2000); delay(100); noTone(PIN_alarm_TONE);
    digitalWrite(PIN_alarm_LED, LOW);
}
