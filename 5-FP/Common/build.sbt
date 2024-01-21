ThisBuild / version := "0.1.0-SNAPSHOT"

ThisBuild / scalaVersion := "2.13.12"

lazy val root = (project in file("."))
  .settings(
    name := "FP",
    // L4
    libraryDependencies += "org.apache.spark" %% "spark-core" % "3.5.0",
    libraryDependencies += "org.apache.spark" %% "spark-sql" % "3.5.0",
    // L5
    libraryDependencies += "mysql" % "mysql-connector-java" % "8.0.33"
  )
