#include "../pinout/pinout.h"
#include "ntc.h"
#include "config.h"

#ifdef USES_NTC

namespace Sensors {

static Poly::DirectMemoryPolyMap<float> *poly10K =
    new Poly::DirectMemoryPolyMap<float>(
        new Poly::Poly<float>[7] {
            Poly::Poly<float>{
                .start = 0.0619,
                .end = 0.2258,
                .deg = 5,
                .coeffs = new float[6]{-106591.78492739468, 109766.42878254162,
                                       -48009.85103429719, 11665.445972632464,
                                       -1769.277109242638, 274.679444983939},
                .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{
                    .start = 0.2258,
                    .end = 0.5083,
                    .deg = 5,
                    .coeffs =
                        new float[6]{-1045.3428889293887, 2631.9864137767445,
                                     -2812.9876768035715, 1643.8082991387028,
                                     -594.1612665462386, 216.50091892000228},
                    .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{
                    .start = 0.5083,
                    .end = 1.2429999999999999,
                    .deg = 5,
                    .coeffs =
                        new float[6]{14.86632629891733, -42.59982726271151,
                                     9.68109139910271, 95.93852811506328,
                                     -157.23703807530762, 166.21515723309716},
                    .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{
                    .start = 1.2429999999999999,
                    .end = 3.592,
                    .deg = 5,
                    .coeffs =
                        new float[6]{-0.14479707737914793, 2.1122112171490994,
                                     -12.737453382005363, 41.443717582846226,
                                     -83.1218508514318, 139.12634660972856},
                    .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{
                    .start = 3.592,
                    .end = 12.47,
                    .deg = 5,
                    .coeffs =
                        new float[6]{-0.00029681208597671135,
                                     0.014478697733427592, -0.29030768994222844,
                                     3.126125832463684, -20.800476916105247,
                                     95.5867687439458},
                    .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{
                    .start = 12.47,
                    .end = 51.82,
                    .deg = 5,
                    .coeffs =
                        new float[6]{-2.7967622152236917e-07,
                                     5.478606039742321e-05,
                                     -0.004350751278526007, 0.1825135902740264,
                                     -4.676918933981655, 57.11293002109976},
                    .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float> {
                .start = 51.82, .end = 277.2, .deg = 5,
                .coeffs =
                    new float[6]{-9.650196480510112e-11, 9.458738788923423e-08,
                                 -3.735203861838748e-05, 0.007626870944630626,
                                 -0.9097465028898091,    21.173082440134568},
                .order = Poly::PolyOrder::HIGHER_POWER_FIRST
            }
        },
        7);

static Poly::DirectMemoryPolyMap<float> *poly100KFull = new Poly::DirectMemoryPolyMap<float> (
new Poly::Poly<float>[8] {
Poly::Poly<float>{.start=0.1056, .end=0.4118, .deg=3, .coeffs=new float [4] {-2276.753664706171,2414.9499099383256,-1006.6999986971276,381.1009906278825}, .order=Poly::PolyOrder::HIGHER_POWER_FIRST},
Poly::Poly<float>{.start=0.4118, .end=0.955, .deg=3, .coeffs=new float [4] {-66.61571810267746,209.70939725996791,-263.88961564643074,295.90058092735785}, .order=Poly::PolyOrder::HIGHER_POWER_FIRST},
Poly::Poly<float>{.start=0.955, .end=2.58, .deg=3, .coeffs=new float [4] {-2.316801319979743,20.913946227985424,-76.38526842533432,232.68627148209362}, .order=Poly::PolyOrder::HIGHER_POWER_FIRST},
Poly::Poly<float>{.start=2.58, .end=8.0283, .deg=3, .coeffs=new float [4] {-0.12137243691389611,2.729421116207972,-25.33315377819568,183.97604577161974}, .order=Poly::PolyOrder::HIGHER_POWER_FIRST},
Poly::Poly<float>{.start=8.0283, .end=32.2059, .deg=3, .coeffs=new float [4] {-0.002190696574915557,0.18171671371354112,-6.025266784199383,131.41828491574677}, .order=Poly::PolyOrder::HIGHER_POWER_FIRST},
Poly::Poly<float>{.start=32.2059, .end=181.6319, .deg=3, .coeffs=new float [4] {-1.2841268051805774e-05,0.005647425816354873,-0.9682065727737157,77.93280080582325}, .order=Poly::PolyOrder::HIGHER_POWER_FIRST},
Poly::Poly<float>{.start=181.6319, .end=522.6908, .deg=3, .coeffs=new float [4] {-2.2711218183901416e-07,0.000340037282081553,-0.20994488110246198,40.203864759831916}, .order=Poly::PolyOrder::HIGHER_POWER_FIRST},
Poly::Poly<float>{.start=522.6908, .end=1733.2, .deg=3, .coeffs=new float [4] {-6.227196615186136e-09,2.9984006155699956e-05,-0.058910345492370696,14.369571462208695}, .order=Poly::PolyOrder::HIGHER_POWER_FIRST}
},
8
);


static Poly::DirectMemoryPolyMap<float> *poly100K =
    new Poly::DirectMemoryPolyMap<float>(
        new Poly::Poly<float>[11] {
            Poly::Poly<float>{
                .start = 3.1860000000000004,
                .end = 5.937,
                .deg = 3,
                .coeffs = new float[4]{-0.17374384269970744, 3.4130818662954723,
                                       -27.960409423115085, 185.0378734146156},
                .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{
                    .start = 5.937,
                    .end = 12.265,
                    .deg = 3,
                    .coeffs =
                        new float[4]{-0.019368841640605216, 0.762101636085383,
                                     -12.498939386497817, 154.3657880986935},
                    .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{.start = 12.265,
                                  .end = 26.568,
                                  .deg = 3,
                                  .coeffs = new float[4]{-0.0017830722700271319,
                                                         0.14956729151050308,
                                                         -5.2378914808707995,
                                                         124.99544734913974},
                                  .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{.start = 26.568,
                                  .end = 65.395,
                                  .deg = 3,
                                  .coeffs = new float[4]{-0.0001250051690658009,
                                                         0.024656676457218488,
                                                         -2.019651688986362,
                                                         96.54119213495831},
                                  .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{.start = 65.395,
                                  .end = 171.545,
                                  .deg = 3,
                                  .coeffs = new float[4]{-6.48830563929323e-06,
                                                         0.0033088676903880523,
                                                         -0.6996354337017481,
                                                         68.3479324967019},
                                  .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{.start = 171.545,
                                  .end = 290.67900000000003,
                                  .deg = 3,
                                  .coeffs = new float[4]{-4.256648809982587e-07,
                                                         0.0005191898918799676,
                                                         -0.26249756745073205,
                                                         44.894088298798124},
                                  .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{.start = 290.67900000000003,
                                  .end = 539.1709999999999,
                                  .deg = 3,
                                  .coeffs = new float[4]{-1.150251425035202e-07,
                                                         0.00020971659730479317,
                                                         -0.16112934328707815,
                                                         33.93429151757367},
                                  .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{
                    .start = 539.1709999999999,
                    .end = 997.2410000000001,
                    .deg = 3,
                    .coeffs =
                        new float[4]{-1.7127936128705894e-08,
                                     5.7534242702636595e-05,
                                     -0.08118323550129387, 19.72367435187801},
                    .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{.start = 997.2410000000001,
                                  .end = 1961.13,
                                  .deg = 3,
                                  .coeffs = new float[4]{-2.258666388866808e-09,
                                                         1.4553014025480438e-05,
                                                         -0.039111516651264076,
                                                         5.761061449958052},
                                  .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float>{.start = 1961.13,
                                  .end = 4197.42,
                                  .deg = 3,
                                  .coeffs = new float[4]{-2.334151481811036e-10,
                                                         3.1124839956678973e-06,
                                                         -0.01715335208683208,
                                                         -8.58467184999908},
                                  .order = Poly::PolyOrder::HIGHER_POWER_FIRST},
                Poly::Poly<float> {
                .start = 4197.42, .end = 11093.8, .deg = 3,
                .coeffs =
                    new float[4]{-1.4297584923230624e-11,
                                 4.6755661356073664e-07, -0.006210056524369176,
                                 -24.15003327868426},
                .order = Poly::PolyOrder::HIGHER_POWER_FIRST
            }
        },
        11);

uint16_t readAnalogPin(uint8_t pin) {
    return (uint16_t)(analogRead(pin)*0.963313 + 200.8358);
}

#ifdef HARDWARE_NTC_10K
    static Poly::DirectMemoryPolyMap<float> *poly = poly10K;
#endif

#ifdef HARDWARE_NTC_100K
    static Poly::DirectMemoryPolyMap<float> *poly = poly100KFull;
#endif


NTC::NTC<float> *ntcController = new NTC::NTC<float>(Pinout::NTC, HARDWARE_NTC_R_PART, poly, 1.0f, NTC::ResistanceRead::RESISTANCE_READ_NTC, 4095, readAnalogPin);

}  // namespace Sensors

#endif

