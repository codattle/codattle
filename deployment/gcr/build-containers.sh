#!/bin/bash

set -e

docker build -t gcr.io/$PROJECT/codattle-core:$TRAVIS_COMMIT -f docker/core/Dockerfile core
docker build -t gcr.io/$PROJECT/codattle-frontend:$TRAVIS_COMMIT -f docker/frontend/Dockerfile frontend

echo $GCLOUD_SERVICE_KEY | base64 --decode -i > gcloud-service-key.json
gcloud auth activate-service-account --key-file gcloud-service-key.json

gcloud --quiet config set project $PROJECT
gcloud --quiet config set container/cluster $CLUSTER
gcloud --quiet config set compute/zone $ZONE
gcloud --quiet container clusters get-credentials $CLUSTER

gcloud docker -- push gcr.io/$PROJECT/codattle-core
gcloud docker -- push gcr.io/$PROJECT/codattle-frontend

yes | gcloud beta container images add-tag gcr.io/$PROJECT/codattle-core:$TRAVIS_COMMIT gcr.io/$PROJECT/codattle-core:latest
yes | gcloud beta container images add-tag gcr.io/$PROJECT/codattle-frontend:$TRAVIS_COMMIT gcr.io/$PROJECT/codattle-frontend:latest