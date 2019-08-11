# Deployment

1. Create Google Cloud project
2. Create Service Account Key https://cloud.google.com/iam/docs/creating-managing-service-account-keys
3. Create infrastructure (Kubernetes cluster and Google Storage Bucket)

```
cd $PROJECT_DIR/codattle/deployment/terraform/ininfrastructure
export GOOGLE_APPLICATION_CREDENTIALS=$PATH_TO_SERVICE_ACCOUNT_KEY
terraform apply
```

4. Create secret

```
cp $PATH_TO_SERVICE_ACCOUNT_KEY google-storage-credentials
kubectl -n codattle create secret generic codattle-secrets --from-file=google-storage-credentials
rm google-storage-credentials
```

5. Install application

```
cd $PROJECT_DIR/codattle/deployment/helm-charts
helm install --namespace codattle --name codattle ./codattle
```
