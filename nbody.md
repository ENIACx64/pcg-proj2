# PCG projekt 2
- autor: Jindřich Vodák
- login: xvodak06

## Měření výkonu (čas / 100 kroků simulace)

### Karolina A100
|   N    |  CPU [s] | Step 0 [s] | Step 1 [s] | Step 2 [s] | Step 3 [s] |
|:------:|:--------:|:----------:|:----------:|:----------:|:----------:|
|   1024 |   0.0409 |     0.0402 |     0.0272 |     0.0274 |            |
|   2048 |   0.0480 |     0.0794 |     0.0544 |     0.0547 |            |
|   4096 |   0.2318 |     0.1573 |     0.1092 |     0.1094 |            |
|   8192 |   0.5964 |     0.3118 |     0.2172 |     0.2175 |            |
|  16384 |   1.6740 |     0.6327 |     0.4629 |     0.4631 |            |
|  32768 |   8.2773 |     1.2655 |     0.9251 |     0.9248 |            |
|  65536 |  26.8663 |     2.6214 |     1.8803 |     1.8808 |            |
| 131072 | 108.4233 |     9.2617 |     7.0675 |     7.0680 |            |
| 262144 | 468.5436 |    32.2956 |    23.8703 |    23.8707 |            |

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
TODO

### Krok 3: Synchronizace
**Popište Vaše řešení synchronizace:**
TODO

**Došlo ke zrychlení?**
TODO

### Krok 4: Měření výkonu
**Jakých jste dosáhli výsledků?**
TODO

**Lze v datech pozorovat nějaké anomálie?**
TODO