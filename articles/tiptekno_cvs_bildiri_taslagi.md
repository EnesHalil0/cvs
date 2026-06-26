# TIPTEKNO Bildiri Ana Taslagi

Calisma basligi icin onerilen ana secenek:

**From Simulink to Embedded C: Real-Time Feasibility of a Seven-State Cardiovascular Model for Hybrid Mock Circuits**

Turkce alternatif:

**Hibrit Mock Circuit Uygulamalari Icin Yedi Durumlu Kardiyovaskuler Modelin Gomulu C Gercek-Zamanli Uygulanabilirliginin Degerlendirilmesi**

## 1. Bildirinin Ana Fikri

Bu bildiri, onceki HMC calismasindaki fiziksel duzenegi veya nonlinear kontrol yapisini yeniden tasarlamayi hedeflememelidir. Ana katkisi daha dar ve TIPTEKNO icin daha uygulanabilir olmalidir:

> Daha once Simulink tabanli olarak kullanilan yedi durumlu kardiyovaskuler sayisal modelin C dilinde yeniden gerceklenmesi, MATLAB/Simulink referans modeline karsi dogrulanmasi, calisma hizi ve zaman adimi/solver secenekleri bakimindan incelenmesi ve STM32 tabanli processor-in-the-loop calismasi icin hazirlanmasi.

Bu kapsam, tip teknolojileri kongresi icin guclu bir muhendislik katkisi sunar: mevcut HMC altyapisini daha dusuk maliyetli, tasinabilir ve gomulu gercek-zamanli calismaya daha yakin bir hesaplama mimarisine tasima.

## 2. Onceki Nonlinear HMC Makalesinden Alinacak Zemin

Nonlinear HMC makalesinde kurulan temel motivasyon su sekildedir:

- VAD gibi mekanik dolasim destek sistemlerinin preklinik testinde sayisal modeller, bench-top mock circuit'ler ve in vivo modeller kullanilir.
- Tam fiziksel mock circuit karmasiklastikca basinclarin, akimlarin ve hacimlerin ayni anda kontrolu zorlasir.
- Hibrit mock circuit yaklasimi model karmasikligini sayisal alanda tutar, fiziksel devreyi daha kurulabilir hale getirir.
- Onceki calismada yedi durumlu CVS modeli dSPACE/Simulink uzerinden calismis, fiziksel pneumo-hydraulic devreye referans basinc uretmis ve Lyapunov/backstepping/Kalman tabanli kontrol yapilari ile izleme saglanmistir.

Yeni bildirinin farki:

- Bu calisma controller tasarimi degil, sayisal CVS modelinin C'ye tasinmasi ve gomulu calismaya hazirlanmasidir.
- Onceki calisma "HMC kontrol edilebilir mi?" sorusunu yanitlarken, bu calisma "HMC'nin sayisal kalbi dSPACE/Simulink disinda, hafif bir C cekirdegi olarak gercek zamanda calisabilir mi?" sorusuna odaklanir.
- Bu nedenle novelty cumlesi su olmali:

> Unlike the previous dSPACE/Simulink-based HMC implementation, this study focuses on an embedded-oriented C reimplementation of the numerical cardiovascular model and evaluates its numerical fidelity, computational speed, solver trade-offs, and STM32 real-time feasibility.

## 3. TIPTEKNO Formatina Uygun Genel Yapi

Yuklenen TIPTEKNO 2026 LaTeX template IEEEtran conference formatindadir. Bu nedenle metin su ana bolumlerle yazilmalidir:

1. Title
2. Abstract
3. Keywords
4. Introduction
5. Materials and Methods
6. Experiments and Evaluation Metrics
7. Results
8. Discussion
9. Conclusion
10. Acknowledgment
11. References

TIPTEKNO bildirilerinde beklenen anlatim genellikle kisa, amaca donuk ve deney/sonuc odaklidir. Bu calisma icin 4-6 sayfalik IEEE conference tarzi bir tam metin hedeflenmelidir.

Kaynak/format gozlemi:

- `Tiptekno2026_Latex_template_ENG.tex` dogrudan IEEEtran conference sinifini kullaniyor.
- `TIPTEKNO24_Tam_Metinler_Kitabi.pdf` ve `TIPTEKNO25_Tam_Metinler_Kitabi.pdf` proceedings yapisinda; kitaplar on bolum, bilimsel program ve tam metin bildirilerden olusuyor.
- Kongre kapsami biyomedikal ve klinik muhendislik uygulamalarini, cihaz/algoritma gelistirmeyi, sinyal/model/deney karsilastirmalarini ve prototip degerlendirmelerini kabul eden bir cerceveye sahip.
- Bu nedenle bu calisma "tam klinik validasyon" iddiasiyla degil, "biyomedikal modelin gomulu uygulamaya yonelik yeniden gerceklenmesi ve referans modele karsi muhendislik dogrulamasi" olarak konumlandirilmalidir.

## 4. Onerilen Abstract Taslagi

**Abstract -** Hybrid cardiovascular mock circuits use numerical cardiovascular models together with physical hydraulic or pneumatic interfaces to create physiologically meaningful test environments for ventricular assist devices. Previous work demonstrated such a system using a dSPACE/Simulink-based seven-state cardiovascular model, nonlinear pressure control, backstepping-based volume regulation, and Kalman filtering. This study focuses on the numerical model component and presents an embedded-oriented C reimplementation of the seven-state lumped-parameter cardiovascular model. The implementation separates ventricular elastance, pressure, flow, and volume-update calculations into a compact fixed-step model core. First, the C Euler implementation is validated against the MATLAB/Simulink reference model operated with the same fixed-step ode1/Euler configuration, initial conditions, time step, and simulation duration. Second, the execution time of the C implementation is benchmarked against the MATLAB/Simulink workflow to quantify computational speed-up and real-time margin. Third, the same C model core is prepared for a solver interface in which Euler can be compared with higher-order alternatives such as Heun or fourth-order Runge-Kutta using accuracy, physiological feature error, and per-step runtime metrics. Finally, the required model refactoring and host-target communication workflow are specified for STM32-based processor-in-the-loop evaluation. The intended outcome is a lightweight and verifiable C model core that reduces dependency on desktop model-based execution tools and provides a practical step toward embedded real-time HMC experimentation.

**Keywords -** cardiovascular model, hybrid mock circuit, ventricular assist device, embedded C, STM32, real-time simulation, processor-in-the-loop

Turkce ozet istenirse:

**Ozet -** Hibrit kardiyovaskuler mock circuit yapilari, sayisal kardiyovaskuler modelleri fiziksel hidrolik veya pnomatik arayuzlerle birlestirerek ventrikuler destek cihazlari icin fizyolojik anlamli test ortamlari sunar. Onceki calismada dSPACE/Simulink tabanli yedi durumlu kardiyovaskuler model, nonlinear basinc kontrolu, backstepping tabanli hacim regülasyonu ve Kalman filtreleme ile kullanilmistir. Bu calisma, soz konusu sistemin sayisal model bilesenine odaklanmakta ve yedi durumlu lumped-parameter kardiyovaskuler modelin gomulu calismaya yonelik C gerceklemesini sunmaktadir. Gercekleme; ventrikuler elastans, basinc, akim ve hacim guncelleme hesaplarini moduler bir sabit adimli model cekirdegi olarak ayirmaktadir. Ilk olarak C-Euler modeli, ayni ode1/Euler konfigurasyonu, baslangic kosullari, zaman adimi ve simülasyon suresi ile MATLAB/Simulink referans modeline karsi dogrulanacaktir. Ikinci olarak C ve MATLAB/Simulink calisma sureleri karsilastirilarak hiz kazanci ve gercek-zaman marji raporlanacaktir. Ucuncu olarak ayni C cekirdegi, Euler ile Heun veya RK4 gibi daha yuksek mertebeli solver alternatiflerini dogruluk, fizyolojik feature hatasi ve adim basina calisma suresi bakimindan karsilastirabilecek bir solver arayuzune hazirlanacaktir. Son olarak STM32 tabanli processor-in-the-loop degerlendirmesi icin gerekli refactor ve host-target haberlesme akisi tanimlanacaktir.

## 5. Introduction Bolumu Nasil Yazilmali?

Introduction bolumu uc paragraflik net bir akisa sahip olmali.

### 5.1 Klinik ve Muhendislik Motivasyonu

Ilk paragrafta VAD/MCA cihazlarinin preklinik test ihtiyaci anlatilmali. Fizyolojik basinc, akim ve hacim dalga sekillerinin sadece statik pompa karakteristigi ile yeterince yakalanamadigi belirtilmeli. Bu kisim onceki nonlinear makalenin girisindeki motivasyonla uyumlu olmali.

Yazilabilecek metin:

> Mechanical circulatory assist devices require controlled preclinical test platforms that can reproduce cardiovascular pressure-flow dynamics under healthy, failing, and assisted conditions. Purely numerical models provide flexibility, whereas purely physical mock circuits provide hardware interaction; however, increasing the physical complexity of a mock circuit makes simultaneous control of pressure, flow, and volume more difficult.

### 5.2 HMC ve Onceki Calismanin Konumu

Ikinci paragrafta HMC yaklasimi tanitilmali. Onceki calisma, yedi durumlu CVS modelini dSPACE/Simulink ortaminda kullanarak fiziksel pneumo-hydraulic devreye referans ureten ve nonlinear kontrol ile izleme yapan bir sistem olarak konumlandirilmali.

Yazilabilecek metin:

> Hybrid mock circuits address this limitation by keeping part of the cardiovascular complexity in silico while preserving a physical hydraulic interface for device testing. A previous HMC implementation used a seventh-order cardiovascular numerical model, dSPACE/Simulink execution, nonlinear pressure control, backstepping-based volume regulation, and Kalman filtering. While this architecture demonstrated accurate pressure tracking and robust operation, its computational core remained tied to desktop/model-based execution tools.

### 5.3 Bu Calismanin Boslugu ve Katkisi

Ucuncu paragraf net contribution listesi vermeli.

Contribution list:

- Yedi durumlu CVS modelinin C dilinde moduler yeniden gerceklenmesi.
- MATLAB/Simulink referansi ile sinyal bazli validation.
- C ve MATLAB/Simulink runtime karsilastirmasi.
- Euler, RK4/Heun ve farkli dt secenekleri ile accuracy/speed trade-off.
- STM32 icin refactor ve processor-in-the-loop hedefi.

Yazilabilecek contribution cumlesi:

> The contribution of this study is an embedded-oriented C reimplementation of the seven-state CVS model, together with a validation and benchmarking workflow designed to quantify numerical agreement with the Simulink reference, computational speed-up, solver/time-step trade-offs, and feasibility for STM32-based real-time execution.

## 6. Materials and Methods Bolumu

Bu bolum bildirinin en onemli bolumu olacak. Asagidaki alt basliklarla yazilmali.

### 6.1 Reference Cardiovascular Model

Bu kisimda Simulink modeli referans kabul edilmeli. Modelin fiziksel anlami ozetlenmeli:

- Seven-state lumped-parameter CVS.
- Compartments/nodes: right atrium (RA), right ventricle (RV), pulmonary circulation (P), left atrium (LA), left ventricle (LV), aorta (AO), veins (V).
- State variables: compartment volumes.
- Derived signals: pressures, outlet flows, ventricular elastances.
- Circulation order: RA -> RV -> P -> LA -> LV -> AO -> V -> RA.
- Initial total blood volume: 5000 mL.
- Fixed-step reference simulation: ode1/Euler, dt = 0.0001 s, duration = 40 s.

Burada bir tablo konmali:

**Table 1. Model states and variables**

| Symbol | Compartment | Unit | Role |
| --- | --- | --- | --- |
| V_RA | Right atrium volume | mL | State |
| V_RV | Right ventricle volume | mL | State |
| V_P | Pulmonary circulation volume | mL | State |
| V_LA | Left atrium volume | mL | State |
| V_LV | Left ventricle volume | mL | State |
| V_AO | Aortic volume | mL | State |
| V_V | Venous volume | mL | State |
| P_i | Compartment pressure | mmHg | Derived |
| Q_i | Outlet flow | mL/s | Derived |
| e_LV, e_RV | Ventricular elastance | mmHg/mL | Time-varying |

### 6.2 C Implementation

Mevcut C kodu soyle anlatilmali:

- `cvs_model.h`: enum, parameter struct, state struct, simulation struct.
- `cvs_elastances.c`: passive elastances, LV time-varying elastance, RV time-varying elastance and phi.
- `cvs_pressures.c`: `P = e * (V - V0)` relation.
- `cvs_flows.c`: pressure-difference-based outlet flows and valve clamping.
- `cvs_volumes.c`: Euler volume update using `Q_in - Q_out`.
- `cvs_output.c`: PC-side CSV output.
- `main.c`: current desktop simulation driver.

Bildiriye koyulacak mimari figuru:

**Figure 1. Software architecture**

```
Simulink Reference Model
        |
        v
Reference CSV Export: V, P, Q
        |
        v
C Model Core
  - Elastance update
  - Pressure update
  - Flow update
  - Volume integration
        |
        v
CSV Output / Metrics / Benchmark
        |
        v
STM32 Step Function + Telemetry
```

Burada onemli ayrim:

- Bildiride "C model is prepared for embedded execution" denebilir.
- Eger refactor henuz tamamlanmadiysa "fully deployed on STM32" denmemeli.
- STM32 olcumu yapilmadan once "processor-in-the-loop target" veya "real-time feasibility analysis" ifadeleri kullanilmali.

### 6.3 Numerical Integration and Solver Configurations

Bu bolum iki ayri deney olarak yazilmali. Simulink karsilastirmasi ile solver karsilastirmasi birbirine karistirilmamali.

**Deney A - Simulink referansina karsi C-Euler dogrulamasi**

Minimum ve asil validation deneyi:

- Euler baseline, dt = 0.0001 s, Simulink `ode1` ile birebir karsilastirma.
- Ayni initial condition, duration, HR ve senaryo flag'leri.
- Bu deneyde yalnizca C-Euler ile Simulink `ode1` karsilastirilmali.
- Amac "C modeli Simulink'teki mevcut modele denk mi?" sorusudur.

**Deney B - C solver trade-off analizi**

- Bu deney Simulink'i ana referans olarak kullanmaz; cunku Simulink burada zaten `ode1` yani Euler ile calistirilmaktadir.
- Euler, Heun ve RK4 gibi C solver'lar ayni model cekirdegi uzerinde karsilastirilir.
- Dogruluk icin bir "high-resolution offline reference" uretilir. Pratik secenek: C-RK4 ile cok kucuk dt, ornegin 0.01 ms veya 0.005 ms, ve `double` precision.
- Her solver icin dt = 1 ms, 0.5 ms, 0.1 ms gibi ayarlar denenir.
- Karsilastirma hem sinyal hatasi hem de fizyolojik feature hatasi uzerinden yapilir.

Metin:

> The baseline validation uses an explicit Euler update in C because the MATLAB/Simulink reference is operated with the fixed-step `ode1` solver. Therefore, the first comparison quantifies whether the C implementation reproduces the existing Simulink model under matched Euler settings. Higher-order solvers are then evaluated as a separate C-only numerical experiment. In this second experiment, Euler, Heun, and RK4 are compared against a high-resolution offline reference to determine whether the computational speed gained by the C implementation can be converted into improved accuracy or larger real-time margin.

### 6.4 Proposed `CVS_Solver` Design

Farkli solver'lar model denklemlerinden ayri tutulmali. C tarafinda hedeflenen yapi su olmali:

```c
typedef enum
{
    CVS_SOLVER_EULER,
    CVS_SOLVER_HEUN,
    CVS_SOLVER_RK4
} CVS_SolverType;

typedef struct
{
    CVS_SolverType type;
    float dt;
} CVS_Solver;

void cvs_model_init(CVS_Model *model, const CVS_Parameters *params);
void cvs_model_step(CVS_Model *model, const CVS_Solver *solver);
```

Tasarim ilkesi:

- Elastance, pressure, flow ve volume derivative hesaplari model core'da kalir.
- Solver yalnizca "state bir sonraki zamana nasil tasinacak?" sorusunu cevaplar.
- Euler Simulink validation icin baseline olarak korunur.
- Heun/RK4 ayni derivative fonksiyonunu birden fazla ara noktada cagirir.
- PC CSV output ve STM32 telemetry solver'dan bagimsiz kalir.

Bu mimari bildiride "embedded-oriented software architecture" katkisini guclendirir.

### 6.5 Validation Workflow

Validation is akisi:

1. Simulink modelini ayni duration, dt ve parameter flags ile calistir.
2. Internal `xpre`, `P_tott`, `Q_tott` sinyallerini CSV olarak export et.
3. C modelini ayni kosulda calistir ve `output.csv` uret.
4. C ciktisini referans zaman vektorune interpolate et.
5. Her sinyal icin MaxAbsError, MAE, RMSE, NRMSE hesapla.
6. LV PV-loop ve pressure waveform overlay figurlari cikar.

Metrikler:

```
e_i(t) = y_C,i(t) - y_ref,i(t)
MaxAbs_i = max_t |e_i(t)|
MAE_i = mean_t |e_i(t)|
RMSE_i = sqrt(mean_t e_i(t)^2)
NRMSE_i = RMSE_i / (max(y_ref,i) - min(y_ref,i))
```

Fizyolojik feature metrikleri:

- MAP: mean arterial pressure.
- Systolic/diastolic AO pressure.
- Stroke volume: max(V_LV) - min(V_LV).
- Ejection fraction: SV / EDV.
- Cardiac output: SV * HR.
- Beat-to-beat period/phase difference.

### 6.6 Runtime Benchmark Workflow

Benchmark deneyleri:

- MATLAB/Simulink runtime: `tic; sim(simInput); toc`.
- C runtime: wall-clock timer veya high-resolution timer.
- Per-step execution time: runtime / number of steps.
- Real-time ratio: simulated time / wall-clock time.
- Deadline margin: `dt / per_step_time`.

Raporlanacak tablo:

| Platform | Solver | dt | Simulated duration | Wall time | Per-step time | Real-time ratio |
| --- | --- | --- | --- | --- | --- | --- |
| Simulink | ode1 | 0.1 ms | 40 s | TBD | TBD | TBD |
| C PC O0 | Euler | 0.1 ms | 40 s | TBD | TBD | TBD |
| C PC O3 | Euler | 0.1 ms | 40 s | TBD | TBD | TBD |
| STM32 | Euler | 1 ms | TBD | TBD | TBD | TBD |

### 6.7 Embedded/STM32 Feasibility

Bu kisimda sunulacak arguman:

- Model cekirdegi dynamic allocation gerektirmiyor.
- `float` kullaniyor.
- State ve parameter arrays statik.
- MCU icin dosya I/O, `printf`, CSV logging ayrilmali.
- Timer interrupt veya RTOS task icinden `cvs_model_step()` cagrilmali.
- Telemetry dusuk oranda USB virtual COM ile gonderilmeli.

STM32 deney plani:

1. C model core'u `main` ve CSV'den ayir.
2. `cvs_model_init()` ve `cvs_model_step()` API'si olustur.
3. STM32CubeIDE projesine yalnizca model core dosyalarini ekle.
4. Timer ile 1 kHz step loop kur.
5. DWT cycle counter veya hardware timer ile step suresini olc.
6. Her N adimda bir state/pressure telemetry gonder.
7. Host PC'de STM32 output ile C/Simulink reference karsilastir.

### 6.8 Processor-in-the-Loop Host Strategy

PIL icin iki olasi host vardir:

**Onerilen pratik yol: Python host + MATLAB/Simulink reference CSV**

- STM32 ile UART/USB virtual COM uzerinden konusmak Python'da daha kolaydir.
- Python `pyserial`, `csv`, `numpy`, `matplotlib` ile parametre gonderme, veri toplama, loglama ve hizli plot islerini rahat yapar.
- MATLAB lisansi veya Simulink oturumu olmadan da PIL deneyi tekrarlanabilir.
- Simulink reference yine MATLAB ile onceden `reference.csv` olarak uretilir; Python host STM32 output'unu bu CSV ile karsilastirir.
- Bildiri icin yeterli ve savunulabilir yol budur.

**Alternatif yol: MATLAB host**

- MATLAB, reference model ve karsilastirma scriptleri zaten orada oldugu icin akademik raporlama acisindan avantajlidir.
- Serial communication MATLAB ile de yapilabilir.
- Ancak real-time host dongusu, paket parsing ve tekrarli deney otomasyonu Python'a gore daha agir ve lisans bagimlidir.

Karar:

> Ilk PIL prototipi Python host ile kurulmalidir. MATLAB/Simulink yalnizca reference CSV uretimi ve final plot/metric dogrulamasi icin kullanilmalidir. Bu ayrim, gomulu calisma iddiasini daha temiz hale getirir: STM32 modeli calistirir, Python veriyi toplar, MATLAB/Simulink ise referans kaynagi olarak kalir.

Onerilen PIL veri akisi:

```
MATLAB/Simulink Reference_CVS
        |
        v
reference.csv
        |
        v
Python PIL Host <---- USB CDC/UART ----> STM32 firmware
        |
        v
stm32_output.csv + metrics.csv + figures
```

## 7. Results Bolumu Nasil Kurgulanmali?

Results bolumu henuz uretilmemis sonuclari kesinmis gibi yazmamali. Deneyler tamamlandiktan sonra asagidaki sirayla doldurulmali.

### 7.1 Model Output Agreement

Verilecek figurlar:

- Figure 2: AO, LV, LA pressure overlay: Simulink vs C.
- Figure 3: LV volume and LV PV-loop overlay.
- Figure 4: Flow signals overlay, ozellikle valve outlet flows.

Yazilacak sonuc tipi:

> Under the matched fixed-step configuration, the C implementation reproduced the main morphology of the Simulink pressure and volume waveforms. The smallest discrepancies were expected in algebraic pressure-volume signals, while flow signals were more sensitive to valve switching instants and numerical alignment.

Mevcut on kontrol notu:

- Mevcut `output.csv` ve `reference.csv` ile kaba karsilastirmada 400000 satir karsilastirildi.
- En kotu MaxAbsError yaklasik olarak volume icin `V_LV = 0.969 mL`, pressure icin `P_LV = 0.310`, flow icin `Q_LA = 250.49` bulundu.
- Bu degerler final sonuc olarak yazilmadan once script ile tekrarlanip tabloya alinmali.

### 7.2 Error Metrics Table

Tablo su formatta olmali:

| Signal group | MaxAbsError | MAE | RMSE | NRMSE |
| --- | --- | --- | --- | --- |
| Volumes | TBD | TBD | TBD | TBD |
| Pressures | TBD | TBD | TBD | TBD |
| Flows | TBD | TBD | TBD | TBD |
| LV PV-loop feature error | TBD | TBD | TBD | TBD |

Ek olarak sinyal bazli tablo appendix veya kisa tabloda verilebilir:

| Signal | MaxAbs | RMSE | Unit |
| --- | --- | --- | --- |
| V_LV | TBD | TBD | mL |
| P_LV | TBD | TBD | mmHg |
| P_AO | TBD | TBD | mmHg |
| Q_LV | TBD | TBD | mL/s |

### 7.3 Runtime Results

Beklenen ana arguman:

- C modelinin Simulink'e gore daha hizli calismasi beklenir.
- Bu hiz kazanci, daha kucuk dt veya daha yuksek mertebeli solver icin kullanilabilir.
- PC uzerinde per-step time, hedef real-time deadline'in cok altinda cikarsa STM32 icin gerekce guclenir.

Yazilacak sonuc tipi:

> The C implementation is expected to provide a substantially lower per-step computational cost than the Simulink reference model because it removes graphical-model execution overhead and uses a compact fixed-size state update. The benchmark will report whether the model satisfies the target deadline for 1 ms and 0.1 ms step sizes on the development PC and, when available, on STM32.

### 7.4 Solver and Time-Step Trade-Off

Raporlanacak grafik:

- x-axis: runtime or per-step time.
- y-axis: RMSE/NRMSE.
- lines: Euler, Heun, RK4.
- markers: dt = 1 ms, 0.5 ms, 0.1 ms.

Beklenen yorum:

- Euler dt azalinca dogruluk artar, runtime artar.
- RK4/Heun ayni dt'de daha iyi dogruluk verebilir ama step maliyeti artar.
- C hizliysa bu maliyet kabul edilebilir olabilir.

Solver'in "daha iyi" oldugunu gostermek icin kullanilacak karar kriterleri:

1. **Sinyal hatasi:** High-resolution offline reference'a gore V/P/Q icin RMSE ve NRMSE daha dusuk olmali.
2. **Fizyolojik feature hatasi:** MAP, systolic/diastolic AO pressure, SV, EF ve CO hatalari daha dusuk olmali.
3. **PV-loop sekli:** LV PV-loop alani ve EDV/ESV noktalarinin referansa yakinligi daha iyi olmali.
4. **Stability/robustness:** Daha buyuk dt'de hacimlerin negatif olmamasi, toplam kan hacminin korunmasi ve sayisal osilasyon olmamasi gerekir.
5. **Runtime/deadline:** Daha iyi solver, hedef step deadline'i asarsa embedded acisindan pratik degildir. Bu nedenle "en iyi" solver yalnizca en dusuk hata veren degil, hata-deadline dengesinde en iyi olan solverdir.

Bu bolumde asil sonuc su sekilde yazilmali:

> Euler remains the correct baseline for reproducing the Simulink ode1 reference. Higher-order solvers are not used to claim better agreement with the ode1 Simulink model; instead, they are evaluated as alternative embedded numerical integration choices. A solver is considered preferable only if it reduces waveform and physiological feature errors relative to a high-resolution offline reference while still satisfying the selected real-time step deadline.

## 8. Discussion Bolumu Nasil Yazilmali?

Discussion bolumu dort ana noktayi tartismali.

### 8.1 Numerical Fidelity

Basinc ve hacimlerde dusuk hata beklenir; flow sinyallerinde valve switching nedeniyle daha yuksek peak error olabilir. Bu bir basarisizlik gibi degil, ayrik zamanli kapak mantigi ve hizli transientlerin hassasiyeti olarak tartisilmali.

Yazilabilecek metin:

> The comparison between C and Simulink should be interpreted at both waveform and feature levels. Small pressure and volume errors indicate that the main cardiovascular dynamics are preserved, whereas larger instantaneous flow errors may arise near valve opening/closing transitions. Therefore, beat-level metrics such as stroke volume, mean arterial pressure, and ejection fraction are important complementary indicators of physiological agreement.

### 8.2 Embedded Value

Asil katki burada:

- dSPACE/Simulink bagimliligi azalir.
- Model daha dusuk maliyetli MCU'da calisabilir hale gelir.
- HMC deneyleri icin daha tasinabilir bir sayisal core elde edilir.

Metin:

> Moving the numerical CVS core from Simulink to C reduces dependency on desktop real-time prototyping hardware and creates a path toward low-cost embedded execution. This is particularly relevant for educational, preliminary device-testing, and portable HMC setups where full dSPACE-class hardware may not be available.

### 8.3 Solver Trade-Off

Bu bolumde "C hiz kazanci yalnizca daha hizli calismak icin degil, daha iyi solver kullanmak icin de harcanabilir" argumani kurulacak.

Metin:

> A faster C implementation may allow the use of smaller time steps or higher-order integration without violating real-time deadlines. This changes the design question from simply matching Simulink to selecting the most appropriate accuracy-cost point for embedded real-time operation.

### 8.4 Limitations

Acik ve durust yazilmali:

- Bu bildiri tam fiziksel HMC kontrolunu yeniden gerceklemez.
- Controller, Kalman filter, pump dynamics ve physical reservoir control kapsam disidir.
- STM32 sonuclari yetismezse sadece feasibility/PC benchmark verilir.
- Model validity, Simulink reference'a gore degerlendirilir; klinik veri ile yeniden kalibrasyon yapilmaz.

Metin:

> This study does not replace the complete HMC control system and does not address pneumatic pressure control, pump control, or sensor filtering. The validation is performed against the existing Simulink reference model rather than against new in vivo or clinical measurements. Therefore, the contribution should be interpreted as an embedded implementation and computational feasibility study of the numerical CVS component.

## 9. Conclusion Taslagi

> This work presents an embedded-oriented C reimplementation of a seven-state cardiovascular numerical model used in a hybrid mock circuit framework. The current implementation separates elastance, pressure, flow, volume, and output calculations and provides a basis for signal-level validation against a MATLAB/Simulink reference model. The planned validation, runtime benchmark, solver comparison, and STM32 timing experiments will quantify whether the C model can preserve numerical fidelity while satisfying real-time execution constraints. The study is expected to support future low-cost processor-in-the-loop HMC experiments and reduce dependency on desktop model-based execution platforms.

## 10. Bildiri Icin Minimum Yapilmasi Gerekenler

Bu liste tamamlanmadan bildiri gonderilmemeli:

- [ ] `make rebuild` ve `./cvs.exe` temiz calismali.
- [ ] Simulink reference CSV ayni dt, duration ve parameter flags ile uretilmeli.
- [ ] C output ve reference icin MaxAbs, MAE, RMSE, NRMSE tablosu uretilmeli.
- [ ] En az pressure ve volume waveform overlay figuru olmali.
- [ ] LV pressure-volume loop comparison figuru olmali.
- [ ] C runtime ve Simulink runtime olculmeli.
- [ ] Per-step time ve real-time ratio hesaplanmali.
- [ ] Bildiride "STM32 port tamamlandi" denecekse gercek STM32 timing sonucu olmali; yoksa "STM32-oriented feasibility" denmeli.

## 11. Bildiriyi Guclendirecek Ek Isler

- [ ] `cvs_model_step()` saf hesaplama API'si ekle.
- [ ] File I/O'yu model core'dan tamamen ayir.
- [ ] `healthy_rest`, `exercise/high HR`, `HFrEF` presetleri ekle.
- [ ] Euler, Heun, RK4 solver secenekleri ekle.
- [ ] dt = 1 ms, 0.5 ms, 0.1 ms karsilastirmasi yap.
- [ ] float vs double accuracy/runtime karsilastirmasi yap.
- [ ] MAP, SV, EF, CO metriklerini cikar.
- [ ] STM32 Nucleo-H755ZI-Q uzerinde DWT cycle counter ile step timing olc.
- [ ] USB VCP telemetry ile host-side comparison yap.

## 12. Adim Adim Calisma Plani

### Hafta 1: Kod ve Validation Cekirdegi

1. `cvs_model_init()` ve `cvs_model_step()` API'si tasarla.
2. `main.c` sadece PC runner olsun.
3. `scripts/compare_metrics.py` ekle.
4. Mevcut MATLAB comparison scriptine RMSE/NRMSE ekle.
5. Reference export ve C run icin tek komutlu workflow kur.

Cikti:

- `output.csv`
- `reference.csv`
- `metrics.csv`
- Pressure/volume/flow overlay figurlari

### Hafta 2: Benchmark ve Ilk Bildiri Taslagi

1. C O0/O3 runtime olc.
2. Simulink runtime olc.
3. Per-step time ve real-time ratio tablosu hazirla.
4. Introduction, Methods, Validation bolumlerini LaTeX'e gecir.
5. Figure 1 workflow ve Figure 2 model diagram hazirla.

Cikti:

- Runtime table
- Draft LaTeX paper
- Minimum results section

### Hafta 3-4: Solver ve Senaryo Analizi

1. Heun veya RK4 ekle.
2. dt sweep calistir.
3. Healthy rest, high HR/exercise, HFrEF senaryolarini kos.
4. Solver accuracy-runtime grafigi hazirla.
5. Discussion bolumunu final sonuclara gore guncelle.

Cikti:

- Solver trade-off table
- Scenario error table
- Discussion draft

### Ay 2: STM32/PIL

1. Model core'u STM32CubeIDE projesine tasi.
2. Timer-driven loop kur.
3. Step time olc.
4. USB telemetry prototipi kur.
5. STM32 output'u host PC reference ile karsilastir.

Cikti:

- STM32 timing result
- PIL workflow figure
- Final paper figure/table set

## 13. Onerilen Figurlar

1. **Workflow diagram:** Simulink reference -> C implementation -> validation -> benchmark -> STM32/PIL.
2. **Seven-state CVS diagram:** RA, RV, P, LA, LV, AO, V circulation loop.
3. **C software architecture:** model core, runner, logging, metrics, embedded layer.
4. **Pressure waveform overlay:** LV/AO pressure, C vs Simulink.
5. **LV PV-loop comparison:** C vs Simulink.
6. **Solver/runtime trade-off:** RMSE vs per-step time.
7. **STM32 timing result:** per-step execution time vs deadline.

## 14. Onerilen Tablolar

1. Model states and units.
2. C module mapping to model equations.
3. C vs Simulink error metrics.
4. Runtime/speed-up comparison.
5. Solver/dt trade-off.
6. STM32 timing/resource result.

## 15. Referans Stratejisi

Minimum referanslar:

- Onceki nonlinear HMC makalesi: HMC, VAD testing, dSPACE/Simulink, nonlinear pressure/volume control zemini.
- Lumped-parameter cardiovascular model veya electrical analogue CVS kaynaklari.
- VAD/mock circuit testing icin temel kaynaklar.
- Embedded real-time/PIL veya model-based design kaynaklari.

Metin icinde kullanilacak referans yerleri:

- Introduction ilk paragraf: VAD/MCA test platformlari.
- HMC paragrafi: onceki nonlinear HMC makalesi.
- Methods model paragrafi: seven-state CVS/electrical analogue kaynaklari.
- Embedded paragrafi: real-time embedded simulation/PIL kaynaklari.

## 16. Dikkat Edilecek Dil ve Iddia Sinirlari

Kullanilabilecek guvenli ifadeler:

- "reimplemented in C"
- "validated against the MATLAB/Simulink reference"
- "prepared for embedded real-time execution"
- "evaluated for STM32 feasibility"
- "processor-in-the-loop target"

Sonuc olmadan kullanilmamasi gereken ifadeler:

- "successfully deployed on STM32"
- "real-time operation was achieved"
- "clinically validated"
- "controller performance was improved"
- "complete HMC system was implemented"

## 17. En Onemli Ilk 5 Aksiyon

1. `cvs_model_step()` refactor'unu yap.
2. RMSE/NRMSE iceren otomatik validation scriptini yaz.
3. C vs Simulink error table ve waveform overlay figurlarini uret.
4. C O3 ve Simulink runtime benchmarklarini al.
5. Sonuclara gore Results ve Discussion bolumlerini sayisal degerlerle doldur.
