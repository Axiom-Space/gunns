pipeline {
    // use parameterized agent.
    agent {
        label params.AGENT == "any" ? "" : params.AGENT
    }
    options {
        // multiple commits on the same branch results in build VM getting bogged down. block multiple builds on same branch
        disableConcurrentBuilds(abortPrevious: true)
        // When we have test-fails e.g. we don't need to run the remaining steps
        buildDiscarder(logRotator(artifactDaysToKeepStr: "5", artifactNumToKeepStr: "25", daysToKeepStr: "5", numToKeepStr: "25"))
        timeout(time: 2, unit: 'HOURS')
    }
    parameters {
        choice(name: "AGENT", choices: ["ipa-slave", "ipa-trick-slave", "any"], description: 'Select Jenkins Node to use' )
    }
    environment {
        BRANCH_NAME="${GIT_BRANCH}"
    }

    stages {
        stage('Run Unit Tests') {
            steps {
                sh '''#!/bin/bash
                    source ./bin/bashrc
                    export JENKINS_RUN="running in jenkins"
                    ./bin/run_jenkins_unit_tests.sh
                   '''
            }
        }
    }
    post {
        always {
            script {  // Make sure path is avail. Bug prevents avail path in some conditions
                if (getContext(hudson.FilePath)) {
                    // Clean after build
                    cleanWs(cleanWhenNotBuilt: false,
                            deleteDirs: true,
                            disableDeferredWipeout: true,
                            notFailBuild: true,
                            patterns: [[pattern: '.gitignore', type: 'INCLUDE']])
                }
            }
        }
    }
}
