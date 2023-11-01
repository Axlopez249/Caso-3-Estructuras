# Usa una imagen base de C++ y un sistema operativo ligero
FROM gcc:latest

# Instala las dependencias necesarias para tu aplicación (en este caso, el paquete libcurl)
RUN apt-get update && apt-get install -y libcurl4-openssl-dev

# Establece el directorio de trabajo
WORKDIR /app

# Copia todos los archivos en el directorio actual al contenedor
COPY . /app

# Copia el código fuente de httplib y json.hpp
COPY httplib.h /app/httplib.h
COPY json.hpp /app/json.hpp

# Compila tu aplicación junto con httplib y json.hpp
RUN g++ -o server PruebaServidor.cpp /app/httplib.h /app/json.hpp -lcurl

# Expón el puerto en el que tu servidor escuchará (puerto 8080 en este caso)
EXPOSE 8080

# Comando predeterminado para ejecutar tu servidor
CMD ["./server"]
