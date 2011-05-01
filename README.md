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
Morphing je kombinaci warpingu dvou obrázků proti sobě a jejich následného blendingu.

Na počátečním a koncovém obrázku je nejdříve vyznačena síť, která vytváří konvexní čtyřúhelníkové dlaždice (na každém obrázku jinak, ale hustota sítě je stejná). Ideálně by uzly této sítě měly vyznačovat stejná místa na obou obrázcích (oči, ústa, apod.). Během morphingu je pak původní obrázek postupně warpován na síť koncového obrázku, a naopak koncový obrázek je nejdříve zwarpován podle sítě počátečního obrázku a následně se vrací do normálu. Warpované obrázky jsou navíc během morphingu blendovány s různými koeficienty tak, aby jeden přecházel do druhého.

Při vhodně zvolených uzlových bodech je tak výsledkem morphingu např. plynulý přechod jednoho obličeje do druhého. Kvalita morphingu pak závisí především na hustotě sítě.

### Warping ###
![Síťový warping](warping.jpg)

Na obrázku výše můžete vidět jak postupně probíhá síťový warping. Warping je vlastně libovolná obecná geometrická transformace obrazu. Poněvadž pracujeme se čtyřúhelníky, rozhodli jsme se použít operace perspektivní transformace. Ta je totiž v knihovně OpenCV velice dobře podporována. Stačí pomocí funkce `getPerspectiveTransform` ze dvou polí se čtyřmi body vygenerovat transformační matici, a poté pomocí funkce `warpPerspective` provést samotnou transformaci. Následně jen stačí zadaný polygon vymaskovat, a přidat k výslednému obrazu.

Bohužel, perspektivní korekce neprovádí změny tak lokálně, jak bychom rádi. Pokud na obdélníku změníme pouze jeden jeho vrchol, celý obrázek se *zkroutí*. Na obrázku výše si tohoto jevu můžete povšimnout na nenavazujících hranách klobouku a ramene. Tato vada ovšem není tak patrná, pokud je síť dostatečně hustá, a změny v sítích nejsou příliš velké. Tudíž pro morphing dvou obličejů je perspektivní transformace dostačující.

Také jsme přemýšleli o vyžití mapování mapování textur v OpenGL, ale nakonec jsme tuto variantu zavrhli, a zůstali u knihovny OpenCV.

Popis obsluhy programu
----------------------
![Screenshot programu](screenshot.jpg)

Po spuštění programu `./morphing` se zobrazí hlavní okno. Aby bylo možné vyzkoušet, jak aplikace pěkně pracuje, je nutné nejprve otevřít dva soubory (obrázky), se stejnými rozměry (např: 512x512). K tomu slouží položky "Load source file" a "Load destination file", které se nachází ve "File" menu v hlavní nabídce programu. Po úspěšném otevření obou souborů je možné přejít k nastaveni mřížky. Mřížku lze upravit v okně, které se otevře při výběru "Set source grid" resp. "Set destination grid" z "Tool" menu v hlavní nabídce programu. 

#### Nastavení mřížky ####
Nejdříve je vhodné si nastavit počet sloupců a řádků mřížky. K tomu slouží tlačítka "Add" a "Remove" v leve části okna. Poté je možne upravit rozmístění jednotlivých uzlů (průsečíků) mřížky. To provedeme přetažením jednotlivých uzlů pomocí kurzoru myši. Tím nastavíme klíčové body na pozice v obrázku.

#### Morphing ####
Po nastavení mřížky a rozmístění bodů v source i destination souboru můžeme přejít k zobrazení morphingu. Ve spodní části hlavního okna se nachází posuvník, kterým je možné se posouvat po krocích a vidět tak průběh morphingu v dané části.

#### Panel nástrojů ####
Na panelu nástrojů v hlavním kně programu se nachází tlačítko "Save as", pomocí kterého lze uložit obrázek morphingu v právě nastaveném kroce. Dále je možně obrázek, který se zobrazuje v okně, pro lepší viditelnost zoomovat a nebo nastavit "Best fit". Při tomto nastaveni se zobrazí vždy celý obrázek do okna.

Zhodnocení výsledků
-------------------
TODO

Použitá literatura
------------------
* [OpenCV 2.1 C++ Reference](http://opencv.willowgarage.com/documentation/cpp/index.html)
* [Transformation matrix - Wikipedia](http://en.wikipedia.org/wiki/Transformation_matrix)
* [GTK+ 3 Reference Manual](http://developer.gnome.org/gtk3/stable/)
