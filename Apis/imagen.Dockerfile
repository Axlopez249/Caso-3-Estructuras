# Usa una imagen base de C++ y un sistema operativo ligero
FROM gcc:latest

# Instala las dependencias necesarias para tu aplicación (en este caso, el paquete libcurl)
RUN apt-get update && apt-get install -y libcurl4-openssl-dev

# Establece el directorio de trabajo
WORKDIR /app

# Copia todos los archivos en el directorio actual al contenedor
COPY . /app

# Compila tu aplicación junto con httplib y otras dependencias
RUN g++ -o server server.cpp ArbolTitulo.cpp ChatGPT.cpp -lcurl

# Expón el puerto en el que tu servidor escuchará (puerto 8080 en este caso)
EXPOSE 8080

# Comando predeterminado para ejecutar tu servidor
CMD ["./server"]
