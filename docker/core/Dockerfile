FROM openjdk:8u191-alpine
WORKDIR /usr/src/app
COPY gradle gradle
COPY gradlew ./
RUN ./gradlew
COPY annotation-processor/src annotation-processor/src
COPY annotation-processor/build.gradle annotation-processor/build.gradle
COPY src src
COPY build.gradle gradle.properties settings.gradle ./
RUN ./gradlew shadowJar

FROM openjdk:8u191-alpine
COPY --from=0 /usr/src/app/build/libs/core-*.jar /app/core.jar
CMD ["java", "-jar", "/app/core.jar"]