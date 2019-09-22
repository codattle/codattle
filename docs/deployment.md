# Deployment

1. Create Google Cloud project
2. Create Service Account Key https://cloud.google.com/iam/docs/creating-managing-service-account-keys
3. Create infrastructure (Kubernetes cluster and Google Storage Bucket)

   ```bash
   cd $PROJECT_DIR/codattle/deployment/terraform/ininfrastructure
   export GOOGLE_APPLICATION_CREDENTIALS=$PATH_TO_SERVICE_ACCOUNT_KEY
   terraform apply
   ```

4. Configure kubectl

   ```bash
   gcloud container clusters get-credentials codattle-kubernetes --zone europe-west2-a
   ```

   To see all projects

   ```bash
   gcloud container clusters list
   ```

5. Create namespace

   ```bash
   kubectl create namespace codattle
   ```

6. Create secrets

   ```bash
   cp $PATH_TO_SERVICE_ACCOUNT_KEY google-storage-credentials
   kubectl -n codattle create secret generic codattle-secrets --from-file=google-storage-credentials
   rm google-storage-credentials

   kubectl -n codattle create secret generic realm-secret --from-file=$PROJECT_DIR/deployment/config/realm.json
   ```

7. Install application

   ```bash
   cd $PROJECT_DIR/codattle/deployment/helm-charts
   helm init
   helm install --namespace codattle --name codattle ./codattle
   ```
