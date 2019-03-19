FROM node:10.15.3
WORKDIR /usr/src/app
COPY package.json package-lock.json ./
RUN npm install
COPY tsconfig.json ./
COPY src src
RUN npm run build
ENV NODE_ENV production
CMD ["node", "./build/index.js"]