# Quotes Service

## Design

The project consists of three components:
- IEX Poller (C++) requests companies and their quotes from the IEX and stores them into a database.
- Database (PostgreSQL).
- Server (Golang) provides RESTful API for retrieving stored data.

The following Google Cloud services are used:
- Cloud SQL
- Kubernetes Engine

What's to be done to be production-ready:
- Unit and intergation tests.
- Validation of inputs (configuration, IEX responses, HTTP requests).
- Metrics/monitoring, logs.
- Proper shutdown.
- Error handling.
- Store prices as multiplied integer numbers.
- Possibly, master-slave replication or/and sharding.
- Smarter migration (e.g, using https://github.com/rubenv/sql-migrate).
- Scheduled updates of companies info.
- Database cleanup (remove old quotes).
- Code refactoring (in particular, split main.go into multiple files).
- Use code generation instead of reflection in Golang.
- Possibly, cache companies info in RAM (server component).
- Configure CI process.
- Keep sensitive information (tokens, passwords) in Kubernetes secrets.
- Use Ingress to expose service to the outside world.
- Versioning.

## Installation

This section describes the steps required to deploy both locally and to the cloud.
First of all, obtain the project and change into the project directory:
```bash
$ git clone https://github.com/frolovpeakwork/stock-quotes.git
$ cd stock-quotes
```
Note, that some of the commands described below are supposed to be run from inside the project directory.

### Project setup

Create a Google Cloud project:
- Go to https://console.cloud.google.com and sign in.
- Create a Google Cloud Platform project with a name *stock-quotes*.
- Make sure that billing is enabled for the project.

Enable the following APIs:
- Cloud Shell API
- Google Cloud SQL Admin API
- Google Kubernetes Engine API

To enable an API, use the following instructions:
- In the `Products & services` menu on the left, click on APIs and Services > Library.
- Type in the name of the API you are searching for in the search bar.
- Click the API you would like to enable, and then click the Enable button.

Before you proceed, you must have:
- The gcloud command-line tool installed and authorized.
- Set the default project for the gcloud tool to the *stock-quotes*.

### PostgreSQL setup

Create a Cloud SQL PostgreSQL instance:
```bash
$ gcloud sql instances create postgres \
  --zone europe-west6-a \
  --database-version POSTGRES_9_6 \
  --memory 4 \
  --cpu 2
```

Create the schema where the application will store the data:
```bash
$ gcloud sql databases create quotes --instance=postgres
```

Next, we need to set the password for the postgres user.
We will use this user to connect our application to the database:
```bash
$ gcloud sql users set-password postgres \
   --instance postgres \
   --password [PASSWORD]
```

### Database access setup

Create a new service account with the following command:
```bash
$ gcloud iam service-accounts create proxy-user --display-name "cloud-sql-admin"
```

Verify the email of the service account, which will be used in the following steps:
```bash
$ gcloud iam service-accounts list
```

Next, grant your service account the CloudSQL Client role.
This will allow the account to connect the proxy on your behalf:
```bash
$ gcloud projects add-iam-policy-binding [PROJECT_ID] \
  --member serviceAccount:[SERVICE_ACCOUNT_EMAIL] \
  --role roles/cloudsql.client
```

Create a file called credentials.json that will be used to authenticate with your service account:
```bash
$ gcloud iam service-accounts keys create credentials.json --iam-account [SERVICE_ACCOUNT_EMAIL]
```

## Google Cloud

The following steps are specific for deploy to Google Cloud.

### Kubernetes cluster setup

Create a Kubernetes cluster:
```bash
$ gcloud container clusters create quotes-cluster --zone europe-west6-a
```

Use the following command to authenticate kubectl:
```bash
$ gcloud container clusters get-credentials quotes-cluster --zone europe-west6-a
```

### Creating secrets

In order for our container to access the proxy connection or database, 
we need to create secrets that Kubernetes will pass to the containers as environment variables.
We can turn the credentials.json file into a Kubernetes secret, which will then be used to connect the proxy from within a container:
```bash
$ kubectl create secret generic cloudsql-instance-credentials \
  --from-file=credentials.json=./credentials.json
```

Provide the PostgreSQL credentials for the *postgres* role:
```bash
$ kubectl create secret generic postgres-credentials --from-literal PGPASSWORD=[PASSWORD]
```

### Deploy

Deploy the service:
```bash
$ kubectl apply -f k8s/
```

Next, you need to create a service that exposes your deployment to the outside world:
```bash
$ kubectl expose deployment server-deployment --type "LoadBalancer" --port 8080 --target-port 8080
```

After a few minutes, you can describe the service to get the LoadBalancer Ingress:
```bash
$ kubectl describe services server-deployment | grep "LoadBalancer Ingress"
```

Your service should be ready at http://[LoadBalancer Ingress]:8080.
Navigate to your URL and make some requests, e.g.:
- http://[LoadBalancer Ingress]:8080/quotes?symbols=aapl,fb
- http://[LoadBalancer Ingress]:8080/quotes?symbols=aapl,fb&from=1578655630&to=1632825332

Both 'from' and 'to' parameters are UNIX timestamps in seconds and are optional.

## Local

To run locally you need the Minikube instance up and running:
```bash
$ minikube start
```

The steps are partially the same as those of deploying to the cloud.
Create the secrets:
```bash
$ kubectl create secret generic cloudsql-instance-credentials \
  --from-file=credentials.json=./credentials.json
$ kubectl create secret generic postgres-credentials --from-literal PGPASSWORD=[PASSWORD]
```

Deploy the project:
```bash
$ kubectl apply -f k8s/
```

Wait until all the pods will be in the 'Running' status:
```bash
$ kubectl get pods
```

Expose the service to the outside world:
```bash
$ kubectl expose deployment server-deployment --type=NodePort
```

Open the service in a browser:
```bash
$ minikube service server-deployment
```

Request examples:
- http://[ENDPOINT]/quotes?symbols=aapl,fb
- http://[ENDPOINT]/quotes?symbols=aapl,fb&from=1578655630&to=1632825332
