[ This file explains why there is a incomplete es_ES.po in adition
  to the es.po; that is on purpose ]

El archivo para el locale es_ES solo existe porque en este paises se usa
una representaci�n de los n�meros diferente de la que se usa en los dem�s
paises de habla espa�ola, en el continente americano usan el sistema
anglosaj�n, esdecir 1,000 en vez de 1.000 para 'mil' y como son m�s los paises
que usan el sistema anglosaj�n se ha dejado esa representaci�n en el es.po.

Por ello solo es necesario traducir aquellas ocurrencias donde aparezcan
n�meros; no hace falta perder el tiempo traduciendo en doble lo dem�s;
el sistema de internacionalizaci�n de GNU gettext es lo suficientemente
inteligente como para buscar las traducciones en la locale 'es' cuando no
las encuentra en una 'es_ES' m�s espec�fica.

Tambi�n podriase crear un es_ES para traducir aquellos terminos que difieren
en Espa�a y Am�rica (ej: computadora/ordenador, archivo/fichero, �cono/icono),
si alg�n espa�ol tiene ganas de encargarse de ello, bienvenido.
Notese que solo es necesario traducir aquellas cadenas de texto para las
cuales se usan terminos diferentes en Espa�a; no es necesario traducir las
dem�s, con que est�n en la traducci�n general 'es' ya basta. 
 
Pablo Saratxaga
<srtxg@chanae.alphanet.ch>

Actualizado por
Carlos Perell� Mar�n
<carlos@gnome-db.org>

