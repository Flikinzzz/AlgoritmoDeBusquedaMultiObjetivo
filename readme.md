# BUSQUEDA BI-OBJETIVO

*En esta ocasion como Equipo de Proyecto, nos tomamos el tiempo para investigar toda la carpeta (BOAstar-Windows), material entregado por el docente Carlos Hernandez.*

*Primero entendimos bien el codigo BOAstar donde concluimos lo siguiente:*

*Definición de Estructuras de Datos:
gnode: Representa cada nodo del grafo, almacenando información como un identificador (id), una clave de búsqueda (key), y variables relacionadas con la distancia estimada a la meta.
snode: Representa los nodos de búsqueda específicos de BOA*, manteniendo el estado de cada nodo, su posición en el árbol de búsqueda, y métricas de costo acumuladas.
Tablas de Adyacencia y Configuración Inicial:*

*Las tablas adjacent_table y pred_adjacent_table guardan las relaciones de adyacencia entre los nodos, es decir, las conexiones directas entre ellos y sus costos. La función read_adjacent_table se encarga de leer la estructura de conexiones a partir de un archivo de entrada, construyendo un modelo del grafo a partir de estos datos.*

*Colas de Prioridad con Heaps Binarios:
BOA* requiere optimizar la inserción y extracción de nodos con la clave más baja. Para ello, el código implementa un heap binario en el que los nodos se ordenan según su estimación de costo total (key). Las funciones insertheap, deleteheap, popheap, y percolateupordown permiten manipular estos heaps de forma eficiente.*

*Algoritmo de Búsqueda de BOA*:*
*La función boastar implementa el núcleo del algoritmo BOA*. Inicializa la búsqueda partiendo del nodo de inicio (start_node) y explorando sucesivamente los nodos más prometedores en el grafo.*
*Utiliza una versión del algoritmo de Dijkstra en la función backward_dijkstra, que optimiza los valores heurísticos (h1 y h2) para guiar la búsqueda, asegurando una exploración eficiente y minimizando el número de nodos expandidos.*

*Durante la búsqueda, se verifican ciertas condiciones de poda para evitar expandir nodos que ya han sido explorados con un costo menor. Los nodos innecesarios se reciclan para optimizar la memoria.*
*Salida y Almacenamiento de Soluciones:*
*El algoritmo almacena todas las soluciones en la matriz solutions y actualiza variables de estadística como stat_expansions (nodos expandidos), stat_generated (nodos generados) y stat_pruned (nodos podados). En caso de encontrar soluciones óptimas, el código las imprime y, si es necesario, las escribe en un archivo de salida.*

*Optimización del Espacio y la Memoria:
El código utiliza un enfoque de reciclaje de nodos podados para liberar memoria de manera eficiente y permite ajustar parámetros de capacidad máxima como MAX_SOLUTIONS y MAX_RECYCLE para gestionar grandes conjuntos de datos.*

# Conclusión

*El artículo demuestra la importancia de usar heurísticas para resolver problemas de rutas multiobjetivo en mapas de carreteras de manera eficiente.  Si bien existen varias opciones, la heurística TC con cálculo acotado se destaca como la más efectiva.*

¿Por qué es tan buena la heurística TC con cálculo acotado?

*Rapidez: Supera a otras heurísticas como la CD y HO, y a la búsqueda a ciegas, especialmente en mapas grandes y complejos como los de Bayreuth (BAY) y Colonia (COL) del conjunto de datos DIMACS.
Eficiencia: Aunque requiere un cálculo inicial (precalcular valores heurísticos), este esfuerzo se ve compensado por la rapidez con la que encuentra la ruta óptima, ahorrando tiempo en la búsqueda general.
Adaptabilidad: El cálculo acotado permite que la heurística se adapte a la dificultad del problema. En problemas simples, donde los puntos de origen y destino están cerca, se precalculan menos valores, ahorrando tiempo. En problemas complejos con puntos de origen y destino lejanos, se precalculan más valores para asegurar la eficiencia.*
