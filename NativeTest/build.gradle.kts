import org.gradle.internal.jvm.Jvm

plugins {
    id("java")
    id("application")
    id("c")
}

mainClassName = 'Main'

group = "org.example"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

application {
    applicationDefaultJvmArgs = ["-Djava.library.path=" + file("${buildDir}/libs/hello/shared").absolutePath]
}

dependencies {
    testImplementation(platform("org.junit:junit-bom:5.9.1"))
    testImplementation("org.junit.jupiter:junit-jupiter")
}


model {
    platforms {
        x64 {
            architecture "x86_64"
        }
    }

    components {
        hello(NativeLibrarySpec) {
            targetPlatform "x64"
            binaries.all {
                def jvmHome = Jvm.current().javaHome
                if (targetPlatform.operatingSystem.macOsX) {
                    cCompiler.args '-I', "${jvmHome}/include"
                    cCompiler.args '-I', "${jvmHome}/include/darwin"
                    cCompiler.args '-mmacosx-version-min=10.4'
                    linker.args '-mmacosx-version-min=10.4'
                } else if (targetPlatform.operatingSystem.linux) {
                    cCompiler.args '-I', "${jvmHome}/include"
                    cCompiler.args '-I', "${jvmHome}/include/linux"
                    cCompiler.args '-D_FILE_OFFSET_BITS=64'
                } else if (targetPlatform.operatingSystem.windows) {
                    cCompiler.args "-I${jvmHome}/include"
                    cCompiler.args "-I${jvmHome}/include/win32"
                } else if (targetPlatform.operatingSystem.freeBSD) {
                    cCompiler.args '-I', "${jvmHome}/include"
                    cCompiler.args '-I', "${jvmHome}/include/freebsd"
                }
            }
        }
    }
}

classes.dependsOn 'helloSharedLibrary'

tasks.test {
    useJUnitPlatform()
}