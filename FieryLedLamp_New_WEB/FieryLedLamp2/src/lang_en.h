#ifndef LANG_EN_FILE
#define LANG_EN_FILE

#define ENGLISH_NAME "English"
static const char *ENGLISH_TEMPLATE_NAME[]=
{
};
static const char *ENGLISH_EFFECT[]=
{
    "Бeлый cвeт",
	"Аврора",
    "Акварель",
    "Аленький цветочек",
	"Бассейн",
	"Бамбук",
	"Безумие",
	"Блуждающий кубик",
	"Водопад",
	"Водопад 4в1",
	"Волны",
	"Волшебный Фонарик",
	"Вино",
	"Вихри пламени",
	"Вихри разноцветные",
	"Вьюга",
	"Гроза в банке",
	"ДНК",
	"Дым",
	"Дым разноцветный",
	"Дымовые шашки",
	"Жидкая лампа",
	"Жидкая лампа авто",
	"Завиток",
	"Звезды",
	"Зебра",
	"Земля Тикси",
	"Змейки",
	"Источник",
	"Капли на воде",
	"Капли на стекле",
	"Кипение",
	"Кодовый замок",
	"Комета",
	"Комета одноцветная",
	"Комета двойная",
	"Комета тройная",
	"Контакты",
	"Конфетти",
	"Кубик Рубика",
	"Лава",
	"Лавовая лампа",
	"Лампа с мотыльками",
	"Лес",
	"Люмeньep",
	"Магма",
	"Масляные краски",
	"Матрица",
	"Мерцание",
  	"Метоболз",
	"Мечта дизайнера",
	"Мозайка",
	"Moтыльки",
	"Мячики",
	"Мячики без границ",
	"Новогодняя Елка",
	"Ночной Город",
	"Огонь",
	"Огонь 2012",
	"Огонь 2018",
	"Огонь 2020",
	"Огонь 2021",
	"Огoнь верховой",
	"Огoнь парящий",
	"Огонь с искрами",
	"Осадки"
	/*
        case EFF_OSCILLATING:         DYNAMIC_DELAY_TICK { effTimer = millis(); oscillatingRoutine();         Eff_Tick (); }  break;  // ( 64U) Осциллятор
        case EFF_CLOUDS:              HIGH_DELAY_TICK { effTimer = millis(); cloudsNoiseRoutine();         Eff_Tick (); }  break;  // ( 65U) Облака
        case EFF_OCEAN:               HIGH_DELAY_TICK { effTimer = millis(); oceanNoiseRoutine();          Eff_Tick (); }  break;  // ( 66U) Океан
        case EFF_OCTOPUS:             DYNAMIC_DELAY_TICK { effTimer = millis(); Octopus();                    Eff_Tick (); }  break;  // ( 67U) Осьминог
        case EFF_RAINBOW_STRIPE:      HIGH_DELAY_TICK { effTimer = millis(); rainbowStripeNoiseRoutine();  Eff_Tick (); }  break;  // ( 68U) Павлин
        case EFF_HOURGLASS:           DYNAMIC_DELAY_TICK { effTimer = millis(); Hourglass();                  Eff_Tick (); }  break;  // ( 69U) Песочные часы
        case EFF_PAINTBALL:           DYNAMIC_DELAY_TICK { effTimer = millis(); lightBallsRoutine();          Eff_Tick (); }  break;  // ( 70U) Пейнтбол
        case EFF_PICASSO:             DYNAMIC_DELAY_TICK { effTimer = millis(); picassoSelector();            Eff_Tick (); }  break;  // ( 71U) Пикассо
        case EFF_PLASMA:              HIGH_DELAY_TICK { effTimer = millis(); plasmaNoiseRoutine();         Eff_Tick (); }  break;  // ( 72U) Плазма
        case EFF_SPIDER:              LOW_DELAY_TICK { effTimer = millis(); spiderRoutine();              Eff_Tick (); }  break;  // ( 73U) Плазменная лампа
        case EFF_PLASMA_WAVES:        SOFT_DELAY_TICK { effTimer = millis(); Plasma_Waves();               Eff_Tick (); }  break;  // ( 74U) Плазменные волны
        case EFF_FLAME:               LOW_DELAY_TICK { effTimer = millis(); execStringsFlame();           Eff_Tick (); }  break;  // ( 75U) Пламя
        case EFF_PLANETEARTH:         SOFT_DELAY_TICK { effTimer = millis(); PlanetEarth();                Eff_Tick (); }  break;  // ( 76U) Планета Земля
        case EFF_BY_EFFECT:           DYNAMIC_DELAY_TICK { effTimer = millis(); ByEffect();                   Eff_Tick (); }  break;  // ( 77U) Побочный эффект
        case EFF_POPCORN:             LOW_DELAY_TICK { effTimer = millis(); popcornRoutine();             Eff_Tick (); }  break;  // ( 78U) Попкорн
        case EFF_PRISMATA:            LOW_DELAY_TICK { effTimer = millis(); PrismataRoutine();            Eff_Tick (); }  break;  // ( 79U) Призмата
        case EFF_ATTRACT:             DYNAMIC_DELAY_TICK { effTimer = millis(); attractRoutine();             Eff_Tick (); }  break;  // ( 80U) Притяжение
        case EFF_LEAPERS:             DYNAMIC_DELAY_TICK { effTimer = millis(); LeapersRoutine();             Eff_Tick (); }  break;  // ( 81U) Пpыгyны
        case EFF_PULSE:               DYNAMIC_DELAY_TICK { effTimer = millis(); pulseRoutine(2U);             Eff_Tick (); }  break;  // ( 82U) Пульс
        case EFF_PULSE_WHITE:         LOW_DELAY_TICK     { effTimer = millis(); pulseRoutine(8U);             Eff_Tick (); }  break;  // ( 83U) Пульс белый
        case EFF_PULSE_RAINBOW:       DYNAMIC_DELAY_TICK { effTimer = millis(); pulseRoutine(4U);             Eff_Tick (); }  break;  // ( 84U) Пульс радужный
        case EFF_RADIAL_WAWE:         DYNAMIC_DELAY_TICK { effTimer = millis(); RadialWave();                 Eff_Tick (); }  break;  // ( 85U) Радиальная волна
        case EFF_RAINBOW_VER:         DYNAMIC_DELAY_TICK { effTimer = millis(); rainbowRoutine();             Eff_Tick (); }  break;  // ( 86U) Радуга
        case EFF_RAINBOW:             HIGH_DELAY_TICK { effTimer = millis(); rainbowNoiseRoutine();        Eff_Tick (); }  break;  // ( 87U) Радуга 3D
        case EFF_SNAKE:               DYNAMIC_DELAY_TICK { effTimer = millis(); MultipleStream8();            Eff_Tick (); }  break;  // ( 88U) Радужный змей
        case EFF_DANDELIONS:          SOFT_DELAY_TICK { effTimer = millis(); Dandelions();                 Eff_Tick (); }  break;  // ( 89U) Разноцветные одуванчики
        case EFF_RAIN:                DYNAMIC_DELAY_TICK { effTimer = millis(); RainRoutine();                Eff_Tick (); }  break;  // ( 90U) Разноцветный дождь
        case EFF_RIVERS:              DYNAMIC_DELAY_TICK { effTimer = millis(); BotswanaRivers();             Eff_Tick (); }  break;  // ( 91U) Реки Ботсваны
        case EFF_LIGHTERS:            DYNAMIC_DELAY_TICK { effTimer = millis(); lightersRoutine();            Eff_Tick (); }  break;  // ( 92U) Светлячки
        case EFF_LIGHTER_TRACES:      DYNAMIC_DELAY_TICK { effTimer = millis(); ballsRoutine();               Eff_Tick (); }  break;  // ( 93U) Светлячки со шлейфом
        case EFF_FEATHER_CANDLE:      DYNAMIC_DELAY_TICK { effTimer = millis(); FeatherCandleRoutine();       Eff_Tick (); }  break;  // ( 94U) Свеча
        case EFF_AURORA:              HIGH_DELAY_TICK { effTimer = millis(); polarRoutine();               Eff_Tick (); }  break;  // ( 95U) Северное сияние
        case EFF_SERPENTINE:          HIGH_DELAY_TICK { effTimer = millis(); Serpentine();                 Eff_Tick (); }  break;  // ( 96U) Серпантин
        case EFF_SINUSOID3:           HIGH_DELAY_TICK { effTimer = millis(); Sinusoid3Routine();           Eff_Tick (); }  break;  // ( 97U) Синусоид
        case EFF_COLORS:              HIGH_DELAY_TICK { effTimer = millis(); colorsRoutine2();             Eff_Tick (); }  break;  // ( 98U) Смена цвета
        case EFF_SNOW:                DYNAMIC_DELAY_TICK { effTimer = millis(); Snowfall();                   Eff_Tick (); }  break;  // ( 99U) Снегопад
        case EFF_SPECTRUM:            DYNAMIC_DELAY_TICK { effTimer = millis(); Spectrum();                   Eff_Tick (); }  break;  // (100U) Спектрум
        case EFF_SPIRO:               LOW_DELAY_TICK { effTimer = millis(); spiroRoutine();               Eff_Tick (); }  break;  // (101U) Спирали
        case EFF_FLOCK:               LOW_DELAY_TICK { effTimer = millis(); flockRoutine(false);          Eff_Tick (); }  break;  // (102U) Стая
        case EFF_FLOCK_N_PR:          LOW_DELAY_TICK { effTimer = millis(); flockRoutine(true);           Eff_Tick (); }  break;  // (103U) Стая и хищник
        case EFF_ARROWS:              DYNAMIC_DELAY_TICK { effTimer = millis(); arrowsRoutine();              Eff_Tick (); }  break;  // (104U) Стрелки
        case EFF_STROBE:              LOW_DELAY_TICK { effTimer = millis(); StrobeAndDiffusion();         Eff_Tick (); }  break;  // (105U) Строб.Хаос.Дифузия
        case EFF_SHADOWS:             LOW_DELAY_TICK { effTimer = millis(); shadowsRoutine();             Eff_Tick (); }  break;  // (106U) Тени
        case EFF_PACIFIC:             LOW_DELAY_TICK { effTimer = millis(); pacificRoutine();             Eff_Tick (); }  break;  // (107U) Тихий океан
        case EFF_TORNADO:             LOW_DELAY_TICK { effTimer = millis(); Tornado();                    Eff_Tick (); }  break;  // (108U) Торнадо
        case EFF_SIMPLE_RAIN:         DYNAMIC_DELAY_TICK { effTimer = millis(); simpleRain();                 Eff_Tick (); }  break;  // (109U) Tyчкa в банке
        case EFF_FIREWORK:            SOFT_DELAY_TICK { effTimer = millis(); Firework();                   Eff_Tick (); }  break;  // (110U) Фейерверк
        case EFF_FIREWORK_2:          DYNAMIC_DELAY_TICK { effTimer = millis(); fireworksRoutine();           Eff_Tick (); }  break;  // (111U) Фейерверк 2
        case EFF_FAIRY:               DYNAMIC_DELAY_TICK { effTimer = millis(); fairyRoutine();               Eff_Tick (); }  break;  // (112U) Фея
        case EFF_COLOR:               LOW_DELAY_TICK  { effTimer = millis(); colorRoutine();               Eff_Tick (); }  break;  // (113U) Цвет
        case EFF_EFF_COLORED_PYTHON : LOW_DELAY_TICK { effTimer = millis(); Colored_Python();             Eff_Tick (); }  break;  // (114U) Цветной Питон
        case EFF_EFF_SAND:            DYNAMIC_DELAY_TICK { effTimer = millis(); sandRoutine();                Eff_Tick (); }  break;  // (115U) Цветные драже
        case EFF_COLOR_FRIZZLES:      SOFT_DELAY_TICK { effTimer = millis(); ColorFrizzles();              Eff_Tick (); }  break;  // (116U) Цветные кудри
        case EFF_EFF_LOTUS:           DYNAMIC_DELAY_TICK { effTimer = millis(); LotusFlower();                Eff_Tick (); }  break;  // (117U) Цветок лотоса
        case EFF_TURBULENCE:          DYNAMIC_DELAY_TICK { effTimer = millis(); Turbulence();                 Eff_Tick (); }  break;  // (118U) Цифровая турбулентность
        case EFF_SPHERES:             LOW_DELAY_TICK { effTimer = millis(); spheresRoutine();             Eff_Tick (); }  break;  // (119U) Шapы
        case EFF_NEXUS:               LOW_DELAY_TICK { effTimer = millis(); nexusRoutine();               Eff_Tick (); }  break;  // (120U) Nexus
        case EFF_CLOCK:               DYNAMIC_DELAY_TICK { effTimer = millis(); clockRoutine();               Eff_Tick (); }  break;  // (121U) Часы
	*/
};
#endif