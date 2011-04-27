Zpracování obrazu: Morphing
===========================

Řešitelé
--------
* Bc. Jan Kaláb (xkalab00)
* Bc. Jan Lipovský (xlipov00)
* Bc. František Skála (xskala05)

Zadání
------
Vytvořte aplikaci realizující morphing s použitím síťového warpingu, kde řídící síť se skládá z úseček. Navrhněte uživatelsky příjemné rozhraní pro snadnou editaci řídící sítě.

Datum vypracování
-----------------
<p>1. května 2011</p>

Popis řešení projektu
---------------------
Morphing je kombinaci warpingu 2 obrázků proti sobě a jejich následného blendingu.

Na počátečním a koncovém obrázku je nejdříve vyznačena síť, která vytváří konvexní čtyřúhelníkové dlaždice (na každém obrázku jinak, ale hustota sítě je stejná). Ideálně by uzly této sítě měly vyznačovat stejná místa na obou obrázcích (oči, ústa, apod.). Během morphingu je pak původní obrázek postupně warpován na síť koncového obrázku, a naopak koncový obrázek je nejdříve zwarpován podle sítě počátečního obrázku a následně se vrací do normálu. Warpované obrázky jsou navíc během morphingu blendovány s různými koeficienty tak, aby jeden přecházel do druhého.

Při vhodně zvolených uzlových bodech je tak výsledkem morphingu např. plynulý přechod jednoho obličeje do druhého. Kvalita morphingu pak závisí především na hustotě sítě.

Popis obsluhy programu
----------------------
TODO

Zhodnocení výsledků
-------------------
TODO

Použitá literatura
------------------
* [OpenCV 2.1 C++ Reference](http://opencv.willowgarage.com/documentation/cpp/index.html)
