20170227
========

Vamos a construir un juego nuevo con el material gráfico original del proyecto de Enero de 2015 que dejamos on hold para siempre y con la última revisión del motor.

La última revisión de Pantanow Engine trae un scroller capaz de usar tilesets mapeados (nada de tiles automáticos) sin la restricción sobre paletas original (*) y scrollear hasta 2 pixels por frame. No es mucho, pero apenas ocupa tiempo y para el juego que tengo entre manos tampoco es necesario ir más rápido.

(*) *En su encarnación original, el Scroller de Pantanow era capaz de scrollear 8 pixels por frame pero metiendo picos de actividad bastante bestias. Es MUY sencillo cambiarlo para que pueda scrollear 4 pixels por frame (ocupando más frame los que scrollea); si lo necesito puedo hacerlo. Además, la versión original tenía metatiles automáticos (el metatile N está hecho por los 4 patrones 4N y siguientes) y la restricción de que el metatile N de los 16 tenía por fuerza que usar la paleta N>>2.*

Lo primero que tengo que hacer es organizar los tiestos que tengo, principalmente los sets gráficos. Tengo que releerme el hilo para hacer una especie de resumen de ideas, porque seguro que había algunas cojonudas.

Por otro lado, los mapas no llevan compresión (sólo empaquetación: 2 tiles por byte) y ocupan 256*8 = 2048 bytes. El target es NROM, 4 mapas son 8K que es un buen cacho. No sé hasta qué punto será viable gastar más tiempo de frame en un scroller que soporte RLE. 

En el caso de querer soportar RLE podría dividir las tareas de la siguiente forma:

- Descomprimir una tira de 16 metatiles en el buffer.
- Pintar media semifila.
- Pintar media semifila.
- Pintar atributos de toda la fila.

Tendría que reescribir el scroller, pero al ser de solo avance podría hacer streaming. Tendría que codificar el mapa desde abajo hasta arriba e ir comprimiendo por filas. Podría usar hasta 32 tiles si me lo curro, eso me da cuentas de hasta 8 caracteres que está bien para este tipo de mapas. Podría hacer la descompresión como un streaming completo (sin limitar las cuentas a filas) pero eso quizá metería demasiada complejidad...

No sé, tengo tiempo para irme pensando todo esto. Desde luego, un motor así lo molaría todo de cara a Sir Ababol porque poder usar 32 tiles y que los mapas ocupen menos lo mola todo.

Recordemos: este pseudoRLE divide los bytes en 3+5 bits, 3 de cuenta (codificando de 1 a 8 repeticiones) y 5 de número de tile (32 tiles diferentes).

Me parece super atrayente la idea.

~~

Pensemos en la lógica.

Por un lado tengo que tener una forma de extraer un nuevo byte. Un bloque de código al que llamar para traer un nuevo byte descomprimido del stream comprimido.

Por otro lado, hay que rellenar el buffer cuando sea necesario. Hay que tener en cuenta que debe dar tiempo, así que lo suyo es ir 4 pixels por delante, o una tira completa por delante.

Esto significa que tendríamos, para curarnos en salud, que pintar toda una fila de metatiles por encima de la que está apareciendo. Hecho así, tendríamos 16 pixels de ventaja. Esto está bien porque el buffer de colisión es de 256x256 pixels y la pantalla sólo tiene 240 de alto.

No paro de cambiar de idea, pero para esto son las lluvias de ideas. Al final tiraré por el mejor camino, y quien lea esto estará completamente aburrido y me odiará. Por eso no debes leer los diarios de otras personas.

~~

Veamos pues:

- Habrá que rellenar el buffer cuando la posición de la cámara sea múltiplo exacto de 16, o cuando su bit 3 cambie. Almacenemos pues el bit 4 de la cámara y detectemos esto. Cuando esto se detecta -> estado 0, rellenamos el buffer, inicializamos los punteros, y pasamos a estado 1.

- En el estado 1 se pintan 8 metatiles del buffer y se pasa al estado 2.

- En el estado 2 se pintan 8 metatiles del buffer (los siguientes) y se pasa al estado 3.

- En el estado 3 se pintan los 8 semiatributos correspondientes al buffer (parte de arriba o parte de abajo) y se pasa al estado 4.

- En el estado 4 no se hace nada salvo comprobar que cambiamos de fila de tiles, o sea, cuando la posición de la cámara es múltiplo exacto de 8, o cuando su bit 3 cambie. Cuando esto pase -> se pasa al estado 5.

- En el estado 5 se pintan 8 metatiles del buffer y se pasa al estado 6.

- En el estado 6 se pintan 8 metatiles del buffer (los siguientes) y se pasa al estado 7.

- En el estado 7 se rellena el buffer de colisiones.

Esto da una velocidad máxima de efectivamente 4 pixeles.

~~

Con tanto desenrolle esto ocupa un puto huevo pero me da igual. Si me quedo corto de espacios enrollaré.

~~

Buf, he escrito una función tochísima. Para probarla necesito poder comprimir el mapa como yo quiera.

Sigo sin explicarme por qué el mapa está exportado aparte y no se incluye en un .h como un array de C. En serio, no me lo explico ...

~~

Tras un buen ratito la he metido en cintura y funciona genial. Ahora puedo scrollear con RLE, el tiempo que ocupa de frame no es excesivo (aunque por ahora no puedo ir a más de 2 pixels por frame - ¡que es más que suficiente!), y luego he ido y le he puesto a la muñeca.

Lo próximo será examinar las rutinas de enemigos del original e integrarlas. Ahora voy a subir a github y a parar.

~~

El tema de los enemigos funciona como en Pantanow / etc, o sea, con una tira de enemigos que se va consumiendo según avanza el mapa. En el maritrini original es exactamente igual: 3 bytes por enemigo, con el primero la coordenada Y del tile donde aparecen, y 2 bytes con diferentes codificaciones.

Según esto, en el original las codificaciones para estos bytes eran:

	YYYYYYYY XXXXEEEE TTTTSSSS

La codificación de estas cosas es un poco confusa. En realidad todo es muy confuso, como siempre, pero creo que se puede apañar un poco para mejorar el tema.

Necesito un puntero al strip. Además usaré un sistema de slots libres que es siempre mucho más rápido de consumir. 

Se me acaba el tiempo, a ver si por lo menos puedo poner la infraestructura básica (los slots, consumir el strip, estcétera).

	// 1 - Fixed. Shoots.
	// 2 - Back & forth, horizontally. They stop at walls and turn around.
	// 3 - Fallers, like 2 but also affected by gravity
	// 4 - Chasers, they just chase you until you outscreen them.
	// 5 - Arrows. Shot, hit or exit, then respawn.
	// 7 - Platforms. Not an enemy, but still.

Eso es lo que había planeado, no sé qué había y qué no.

~~

Estoy muy cansado, pero he pensado que almacenar el sprite ID es una tontería cuando lo suyo es hacerlo fijo y que tenga que ver con el tipo, directamente. Por tanto:

	YYYYYYYY XXXXTTTT BBBBAAAA

Con AAAA y BBBB codificando diferentes cosas según el tipo:

- Tipo 1, "fixed": AAAA = frequencia (se le hace < que esto a un rand).
- Tipo 2 y tipo 3: AAAA = velocidad en pixels (0 = 1/2 pixel por frame).
- Tipo 4: N/A
- Tipo 5: AAAA = velocidad en pixels, BBBB = orientación; 0 = izq, 1 = der.
- Tipo 7: AAAA = limite 1, BBBB = limite 2.

20170228
========

=Gravedad con valores precalculados=

- eny es el índice de la tabla de incrementos sobre y precalculados.
- Voy a simular una caída en un pequeño programa freeBasic para obtener la tabla de valores de velocidad.

~~

Tipos 2 y 3 están funcionando. Voy a por el 4. Tengo que resolver una colisión de intereses: la colisión vertical reinicia el contador ficticio para que el incremento sea 0; el valor de reinicio será diferente para los tipos 3 y 4. Es un if, lo sé, pero estas cosas me "enfeecen" el conjunto. [*]

~~

=Movimiento perseguidor con valores precalculados=

- Cubro x e y con el mismo lut.
- Simulo en freeBasic, como antes.
- Este modelo es el que usé en Goddess. Si me aturullo, sé donde mirar :*)

~~

Olvidad esto [*] que dije. La tabla sólo lleva valores positivos, la inversión es manual. El 0 sigue estando en el índice 0.

~~

Bueno, tipo 4 metido. He hecho que si los miras no te persigan. Pero el tiempo de frame está subiendo peligrosamente.

Voy a hacer un pequeño benchmark y luego a github.

Quedaría ¿optimizar?, colisiones, disparos.

Los disparos se me comen una barbaridad, son de punto fijo. No sé qué hacer. ¿Funcionaba la implementación en asm que hice en tiempos?

~~

