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

20170301
========

Tengo que pensar en los enemigos que faltan. Por un lado débería comprobar si la implementación en ensamblador de los disparos es lo que se está usando en la demo de hace dos años, porque en ese caso seguro que viene muy bien...

Luego el tema de las flechas. No son más que disparos horizontales, pero no sé si cargar más los disparos. Debería salir una flecha horizontalmente (y muy rápido) cada N frames después de llegar a su destino (de colisionar). Aquí necesitaría estados, y un sprite que ocluya.

De este modo, tendría N frames apareciendo, volando, desapareciendo, idle. Son 4 estados. No sé bien cómo incluir esa infraestructura en lo que tengo, pero seguro que me viene. Lo que pasa es que creo que estoy llegando a mi límite de cansancio, llevo demasiado tiempo durmiendo muy poco, y mi cerebro está realmente lento.

También faltan las plataformas móviles. Quizá debería hacer eso ya, que no es más que comprobar la colisión, y dejar las cosas más complejas para otro momento.

~~

Creo que paso de ensambladores mierder y procesaré las balas solo frames pares e impares dependiendo del número. Si las muevo a 4 pixels no creo que haya condiciones de carrera raras ¿no? El jugador nunca se mueve tan rápido.

Ace.

20170303
========

A ver si pongo colisiones.

- Enemigos [ ]
- Balas [ ]
- Pinches [ ]

~~

Ok, colisiones gestionadas. Ahora tengo que poner el hud. El hud era bastante chulo pero no sé si estaré pillando demasiados sprites con esto...

20170306
========

He pasado el finde dándole fran a otros frentes (un nuevo portátil retro), pero he pensado kosa.

Estaría bien seguir el guión original que habíamos pensado de la película y poner que la primera fase fuese muy sencilla. En ella, Maritrini llega hasta la escena en la que ve a Maite Torras comiéndose un cadáver. Esto debe ser sencillo de hacer, con cosas diseñadas para aprender las mecáncias básicas. Luego, dialoguito, y boss fight. Porque ya va siendo hora de que meta bosses en el juego.

Máquinas de estados, vaya.

El tema sería:

- Hacer avanzar el scroll, una vez que alcancemos una "marca", hasta un punto en el que se alinée al 100% con la pantalla. Esto va a ser lo medio chungo de calcular.

- ¿Por qué es esto? Porque quiero sacar texto, y porque tengo que encontrar la forma de almacenar lo que había en la pantalla antes de mostrar el cuadro de texto para poder restaurarlo. Podría emplear el buffer circular para restaurar la pantalla, pero ahora mismo me da un poco de jiña ponerme a hacer este tipo de cálculos (aunque a lo mejor es mucho más sencillo que lo que tengo pensado).

- La idea es usar el nametable oculto para mostrar el cuadro de texto haciendo DOS splits. Y es aquí donde me entra la jiña. Como es texto fijo puedo hacer los splits usando delays, pero tendría que hacerlo a bajo nivel y tal porque neslib no permite este tipo de manejes (intenté hacer varios scrolls durante el tiempo de frame para ver si podía hacer un parallax perdiendo tiempo en la pantalla de título de maritrini pero un carajo para mí).

- El tema es aprender a hacer scroll vertical por mi cuenta tocando registros y ver si esto es compatible con neslib - o hacerlo compatible con neslib.

- Por supuesto esta es la forma dura pero creo que puedo sacar mucho bien de esto. Habría que perder todo el tiempo necesario hasta la franja de abajo de la zona de texto, y en el cachito que queda, simplemente comprobar que se pulse el botón y esperar a NMI.

¿Y cómo se hace un scroll tocando registros? ¡Ese es el quid!

La rutina de NMI de neslib hace todos los manejes porque el tema tiene su chicha y hay que dejar las cosas bien para que no empiece el wonky wonkeo (yo me entiendo).

	lda #0
	sta PPU_ADDR
	lda #0
	sta PPU_SCROLL
	lda #0
	sta PPU_SCROLL
	lda #0
	sta PPU_ADDR

Voy a probar a hacer esto en un bucle sobre el mismo loop del juego con la pantalla activa a ver k ase.

~~

Oye, con valores fijos medio funciona! Lo único que necesito hacer es asegurarme de que scroll = 0 y que a partir de la fila 4 de la nametable de abajo el texto está bien puesto.

Tengo que buscar buen los valores de conteo porque por ejemplo con 64 baila.

Con un poco de ajuste de tiempo con nops y cosas así lo coloco medio qué.

~~

Ahora el tema está en tener la versión PAL y la NTSC de los valores. El problema principal es el valor inicial, los 64 scanlines que dejo pasar al principio. Usar un valor u otro dependiendo de PAL o NTSC no debería ser un problema, si al final me decanto por esta opción.

Ha sido guay hacer el scroll a mano aunque aún no entiendo bien los valores (en realidad, más o menos, sí, pero mi caso es muy sencillo), pero creo que esta solución queda algo fea. Quizá lo suyo sería de verdad redibujar la pantalla con respecto al buffer.

El tema es:

- Sí o sí, alinear de forma que scroll = 0, o sea, las nametables están "en su sitio".
- A partir de aquí, ver qué fila del buffer coincide con el borde superior de la pantalla, para saber qué redibujar.

Aparte de esto tengo que pensar en cómo voy a implementar el tema de la detección de llegar al sitio y parar en el motor, pero seguro que se me ocurre fási si no me obsesiono.

Ahora habría que definir el boss.

El boss es Mayte Torras transformada en Zombie. Lo suyo es que tenga dos modos:

- Modo disparar, se va al cadaver y te lanza tripas.
- Modo perseguir, te persigue como un fantasma.

Como en un principio Maritrini está indefensa, lo suyo es que active un rayo dándole a un botón. El tema será llevar a la bicha al sitio donde le de el rayo, salir a todo carajo a por el botón, y darle.

El rayo se activa durante 1 segundo, y luego hay que esperar cinco segundos a que el botón se libere y se pueda usar otra vez.

Creo que con esto tengo un primer boss sencillo y efectista.

~~

El tema será ahora dibujar esta primera fase. Debe ser un gimnasio, o la recepción de uno... Lo que sea, pero dividido en pisos.

Con un sencillo divider en el mappy podré controlar que esté realmente alineado a pantalla para construir los pisos.

~~

Qué de cosas hay que resolver. No sé si lo suyo sería pasar esto ya a UNROM o a MMC1 antes de seguir. 

Con MMC1 desde luego podría tener todos los buffers en WRAM que quisiera.

Pero lo bonito es hacer que lo básico funcione en NROM ¿no?

20170307
========

Al final no avancé NADA en ese frente, pero ahí queda apuntado. Por otro lado estuve pensando en poder tener animaciones idle para algunos enemigos y en dotar a Maritrini de algún tipo de hitter para defenderse.

Las animaciones para los enemigos extra tendrían uso por ejemplo en una fase de la ducha que tengo pensada, en la que los enemigos saldrían duchándose y cuando pasas de su "Y" se activarían y ya se pondrían a perseguirte. Podría especificar ese valor extra en el nibble que me queda sin usar y luego usar 32+(N<<2) como ínicio de un bucle de máximo 4 frames (hablo de índice dentro del array de metasprites).

Otras cosas:

- Ver si puedo definir algo más para los enemigos que simplemente andan y caen, como por ejemplo que te persigan horizontalmente (si estás en su nivel (en un rango): se dirigen hacia tí)
- Enemigos "chorro de vapor".
- Conveyors.
- Suelos resbalosos.
- Gotas.
- Implementar las flechas (¿se puede juntar con gotas?).

Muchas cosas que hacer y poco tiempo para hacerlas. Además estuve intentando dibujar la primera fase con el fondo del gimnasio y me estaba quedando de pena. Todo sale muy naranja. Realmente es difícil pelearse con las limitaciones y con los pocos colores, pero sé que lo lograré. Mi problema es que tengo demasiado metido en la cabeza el aspecto de gimnasio clásico victoriano/eduardiano y es mejor que me lo vaya quitando de la cabeza. Espalderas para subir, plataformas gordas, alguna planta, pesas de fondo, ventanales, y poco más.

~~

¿Podría empotriñar gota y flecha en el mismo slot? Los dos van por estados, que son:

- Idle
- Apareciendo.
- "En vuelo"
- Golpeando

Ambos necesitarían una variable que definiese con qué frecuencia salen, en número de segundos desde la última vez.

Recordemos que tenemos 
	
	YYYYYYYY XXXXTTTT BBBBAAAA

El paso de "En vuelo" a "Golpeando" bien puedo hacerlo reaprovechando las colisiones que ya estoy comprobando.

Por tanto necesito:

- Orientación. La gota siempre es para abajo, pero la flecha puede ser izquierda o derecha.
- Tiempo en segundos desde la última vez.
- Euh... ¿ya?

Hum. Si lo hago así voy a forzar que en un nivel sólo pueda haber o gotas o flechas. En realidad tampoco es algo que me despeine si me deja libre un slot de enemigo que luego pueda emplear en algo chulo.

Juer. Estas decisiones me asesinan.

A tomar por culo.

	BBBB = FOxx; AAAA = tiempo.

	Donde F = 1, si flecha.
	O = 1, si izquierda.

Sabiendo esto voy a poner una gota. Tengo que meter los estados. Puedo usar x1 y x2 para ello, que están libres. Debería renombrar estas variables como "aux1" y "aux2" ¿no creen?

~~

Funciona pero creo que voy a tener que ponerme a refactorizar algunas cosas... El tema de los enemigos es un jodido pifostio. Pensaré :)

~~



