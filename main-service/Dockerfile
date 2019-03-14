FROM openjdk:8-jdk-alpine 
RUN apk --no-cache add curl
COPY build/libs/*.jar mainservice.jar
CMD java ${JAVA_OPTS} -jar mainservice.jar