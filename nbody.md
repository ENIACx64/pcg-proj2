# PCG projekt 2
- autor: Jindřich Vodák
- login: xvodak06

## Měření výkonu (čas / 100 kroků simulace)

### Karolina A100
|   N    |  CPU [s] | Step 0 [s] | Step 1 [s] | Step 2 [s] | Step 3 [s] |
|:------:|:--------:|:----------:|:----------:|:----------:|:----------:|
|   1024 |   0.0409 |     0.0402 |     0.0272 |     0.0274 |     0.0450 |
|   2048 |   0.0480 |     0.0794 |     0.0544 |     0.0547 |     0.0735 |
|   4096 |   0.2318 |     0.1573 |     0.1092 |     0.1094 |     0.1305 |
|   8192 |   0.5964 |     0.3118 |     0.2172 |     0.2175 |     0.2435 |
|  16384 |   1.6740 |     0.6327 |     0.4629 |     0.4631 |     0.4973 |
|  32768 |   8.2773 |     1.2655 |     0.9251 |     0.9248 |     0.9861 |
|  65536 |  26.8663 |     2.6214 |     1.8803 |     1.8808 |     1.9686 |
| 131072 | 108.4233 |     9.2617 |     7.0675 |     7.0680 |     7.2390 |
| 262144 | 468.5436 |    32.2956 |    23.8703 |    23.8707 |    24.2008 |

<!---
### Barbora V100 (dobrovolné)
|   N    | Step 0 [s] | Step 1 [s] | Step 2 [s] | Step 3 [s] |
|:------:|:----------:|:----------:|:----------:|:----------:|
|   1024 |            |            |            |            | 
|   2048 |            |            |            |            |
|   4096 |            |            |            |            |
|   8192 |            |            |            |            |
|  16384 |            |            |            |            |
|  32768 |            |            |            |            |
|  65536 |            |            |            |            |
| 131072 |            |            |            |            |
| 262144 |            |            |            |            |
-->

<!---
### Jiné GPU: [MODEL] (dobrovolné)
|   N    | Step 0 [s] | Step 1 [s] | Step 2 [s] | Step 3 [s] |
|:------:|:----------:|:----------:|:----------:|:----------:|
|   1024 |            |            |            |            | 
|   2048 |            |            |            |            |
|   4096 |            |            |            |            |
|   8192 |            |            |            |            |
|  16384 |            |            |            |            |
|  32768 |            |            |            |            |
|  65536 |            |            |            |            |
| 131072 |            |            |            |            |
| 262144 |            |            |            |            |
-->

## Otázky

### Krok 0: Základní implementace
**Vyskytla se nějaká anomále v naměřených časech? Pokud ano, vysvětlete:**
Při velikosti domény N=2048 je naměřený čas pro Step0 vyšší než v případě referenční implementace. To je nejspíš dáno tím, že daná úloha je příliš malá pro efektivní zpracování na GPU, a časové penalizace spojené s režií a datovými přesuny převáží potenciální výhody zpracování na GPU.

### Krok 1: Sloučení kernelů
**Došlo ke zrychlení?**
Ano, ke zrychlení došlo.

**Popište hlavní důvody:**
Step1 měl za úkol sloučit výpočetní kernely. Skrze toto sloučení došlo ke snížení režie spojené s voláním kernelů.

### Krok 2: Výpočet těžiště
**Kolik kernelů je nutné použít k výpočtu?**
Výpočet těžiště zajišťuje jediný kernel centerOfMass().

**Kolik další paměti jste museli naalokovat?**
Byla naalokována paměť o velikosti nebližší mocniny dvojky vyšší než N, tedy např. pro N=1000 by byla naalokována paměť maxN=1024. To umožnilo snažší implementaci redukce.

**Jaké je zrychelní vůči sekveční verzi? Zdůvodněte.**
Ke zrychlení došlo, ale bylo velmi malé (v rámci desetitisícin sekund). Mezi možné důvody může patřit velikost domény či neoptimální způsob implementace redukčního algoritmu.

### Krok 3: Synchronizace
**Popište Vaše řešení synchronizace:**
Implementace synchronizace je principiálně ekvivalentní řešení z prvního projektu. Jsou vytvořeny tři stream ID pro výpočet nové polohy částice, kopírování dat a výpočet těžiště. Tato ID jsou zadefinována v hlavičce v main.cpp i nbody.cpp. Při volání kernelu či metody pro kopírování dat je použit async s ID příslušné operace. Operace jsou synchronizovány pomocí wait. Na konci se zkopírovaná data zapíší do souboru pomocí metod objektu H5Helper.

**Došlo ke zrychlení?**
Ne, ke zrychlení nedošlo.

### Krok 4: Měření výkonu
**Jakých jste dosáhli výsledků?**
Výsledky se dají považovat za poměrně dobré, neboť došlo k výraznému zrychlení oproti referenční implementaci. Zároveň došlo i ke zrychlení oproti CUDA verzi, viz minulý projekt.

**Lze v datech pozorovat nějaké anomálie?**
Mimo anomálii popsanou v části věnované Step0 došlo ještě k anomálii u zpomalení v synchronizační úloze. Zpomalení je pravděpodobně dáno tím, že se operace z důvodu různých složitostí daných operací nedokázaly kompletně překrýt a tím pádem dochází k marnění času.