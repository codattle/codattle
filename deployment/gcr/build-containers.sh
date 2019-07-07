#!/bin/bash

set -e

docker build -t gcr.io/$PROJECT/codattle-core:$TRAVIS_COMMIT -f - core < docker/core/Dockerfile
docker build -t gcr.io/$PROJECT/codattle-frontend:$TRAVIS_COMMIT -f - frontend < docker/frontend/Dockerfile
docker build -t gcr.io/$PROJECT/codattle-simulation:$TRAVIS_COMMIT -f - simulation < docker/simulation/Dockerfile

echo $GCLOUD_SERVICE_KEY | base64 --decode -i > gcloud-service-key.json
gcloud auth activate-service-account --key-file gcloud-service-key.json

gcloud --quiet config set project $PROJECT
gcloud --quiet config set container/cluster $CLUSTER
gcloud --quiet config set compute/zone $ZONE
gcloud --quiet container clusters get-credentials $CLUSTER

gcloud docker -- push gcr.io/$PROJECT/codattle-core
gcloud docker -- push gcr.io/$PROJECT/codattle-frontend
gcloud docker -- push gcr.io/$PROJECT/codattle-simulation

yes | gcloud beta container images add-tag gcr.io/$PROJECT/codattle-core:$TRAVIS_COMMIT gcr.io/$PROJECT/codattle-core:latest
yes | gcloud beta container images add-tag gcr.io/$PROJECT/codattle-frontend:$TRAVIS_COMMIT gcr.io/$PROJECT/codattle-frontend:latest
yes | gcloud beta container images add-tag gcr.io/$PROJECT/codattle-simulation:$TRAVIS_COMMIT gcr.io/$PROJECT/codattle-simulation:latest

kubectl config view
kubectl config current-context

kubectl -n codattle set image deployment/codattle-core codattle-core=gcr.io/$PROJECT/codattle-core:$TRAVIS_COMMIT
kubectl -n codattle set image deployment/codattle-frontend codattle-frontend=gcr.io/$PROJECT/codattle-frontend:$TRAVIS_COMMIT
kubectl -n codattle set image deployment/codattle-simulation codattle-simulation=gcr.io/$PROJECT/codattle-simulation:$TRAVIS_COMMIT