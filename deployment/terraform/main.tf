data "google_client_config" "current" {}

provider "helm" {
  kubernetes {
    host                   = "${module.infrastructure.cluster_endpoint}"
    token                  = "${data.google_client_config.current.access_token}"
    client_certificate     = "${base64decode(module.infrastructure.cluster_client_certificate)}"
    client_key             = "${base64decode(module.infrastructure.cluster_client_key)}"
    cluster_ca_certificate = "${base64decode(module.infrastructure.cluster_ca_certificate)}"
  }
}

module "infrastructure" {
  source = "infrastructure"
}

module "app" {
  source = "app"
}