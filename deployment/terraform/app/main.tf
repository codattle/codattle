resource "helm_repository" "helm-chart-repository" {
    name = "helm-chart-repository"
    url  = "https://github.com/codattle/charts"
}

resource "helm_release" "codattle" {
  name      = "codattle"
  repository = "${helm_repository.helm-chart-repository.metadata.0.name}"
  chart     = "codattle"
}