pluginManagement {
    repositories {
        google()
        maven { url=uri("https://maven.aliyun.com/nexus/content/groups/public/") }
        mavenCentral()
        gradlePluginPortal()
        mavenLocal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        maven { url=uri("https://maven.aliyun.com/nexus/content/groups/public/") }
        mavenCentral()
        mavenLocal()
    }
}

rootProject.name = "JustPlayer"
include(":app")
include(":ffplayer")
