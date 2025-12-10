# PCG projekt 2
- autor: Jindřich Vodák
- login: xvodak06

## Měření výkonu (čas / 100 kroků simulace)

### Karolina A100
|   N    |  CPU [s] | Step 0 [s] | Step 1 [s] | Step 2 [s] | Step 3 [s] |
|:------:|:--------:|:----------:|:----------:|:----------:|:----------:|
|   1024 |   0.0409 |            |            |            |            |
|   2048 |   0.0480 |            |            |            |            |
|   4096 |   0.2318 |            |            |            |            |
|   8192 |   0.5964 |            |            |            |            |
|  16384 |   1.6740 |            |            |            |            |
|  32768 |   8.2773 |            |            |            |            |
|  65536 |  26.8663 |            |            |            |            |
| 131072 | 108.4233 |            |            |            |            |
| 262144 | 468.5436 |            |            |            |            |

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

### Krok 1: Sloučení kernelů
**Došlo ke zrychlení?**

**Popište hlavní důvody:**

### Krok 2: Výpočet těžiště
**Kolik kernelů je nutné použít k výpočtu?**

**Kolik další paměti jste museli naalokovat?**

**Jaké je zrychelní vůči sekveční verzi? Zdůvodněte.**

### Krok 3: Synchronizace
**Popište Vaše řešení synchronizace:**

**Došlo ke zrychlení?**

### Krok 4: Měření výkonu
**Jakých jste dosáhli výsledků?**

**Lze v datech pozorovat nějaké anomálie?**