

/*

https://es.cppreference.com/w/cpp/language/template_specialization

clase llamada output<T>, con el objetivo de trabajar both con strings y listas utilizando una misma interfaz

la interfaz deberia ser aproximadamente

public añadir token (recibe un string y lo mete)
public obtener representacion (devuelve la lista o string)

el formato del string es:

token1
token2
token3
token4
...

no olvidarse de crear output.h y hacer include directo del cpp en tokenizador para que compile


*/
