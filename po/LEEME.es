[ This file explains why there is a incomplete es_ES.po in adition
  to the es.po; that is on purpose ]

El archivo para el locale es_ES solo existe porque en este paises se usa
una representación de los números diferente de la que se usa en los demás
paises de habla española, en el continente americano usan el sistema
anglosajón, esdecir 1,000 en vez de 1.000 para 'mil' y como son más los paises
que usan el sistema anglosajón se ha dejado esa representación en el es.po.

Por ello solo es necesario traducir aquellas ocurrencias donde aparezcan
números; no hace falta perder el tiempo traduciendo en doble lo demás;
el sistema de internacionalización de GNU gettext es lo suficientemente
inteligente como para buscar las traducciones en la locale 'es' cuando no
las encuentra en una 'es_ES' más específica.

También podriase crear un es_ES para traducir aquellos terminos que difieren
en España y América (ej: computadora/ordenador, archivo/fichero, ícono/icono),
si algún español tiene ganas de encargarse de ello, bienvenido.
Notese que solo es necesario traducir aquellas cadenas de texto para las
cuales se usan terminos diferentes en España; no es necesario traducir las
demás, con que estén en la traducción general 'es' ya basta. 
 
Pablo Saratxaga
<srtxg@chanae.alphanet.ch>

Actualizado por
Carlos Perelló Marín
<carlos@gnome-db.org>

