FROM node:10.15.3
WORKDIR /usr/src/app
COPY package.json package-lock.json ./
RUN npm install
COPY bsconfig.json graphql_schema.json webpack.config.js .env.prod ./
COPY src src
RUN npm run build
RUN npm run webpack:production

FROM nginx:1.15.9-alpine
COPY --from=0 /usr/src/app/build /usr/share/nginx/html
COPY nginx/default.conf /etc/nginx/conf.d/