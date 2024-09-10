use rocket::*;
use rocket::serde::json::Json;
use rocket::serde::json::serde_json::json;
use rocket_db_pools::Connection;
use rocket_db_pools::diesel::prelude::*;

use crate::MyDatabase;
use crud_rs::ApiResponse;

use crate::schema::service;
use crate::models::service::{NewService, Service, UpdateService};

#[post("/service", format = "application/json", data = "<service>")]
pub async fn create(mut db: Connection<MyDatabase>, service: Json<NewService>) -> ApiResponse {
    let response =
        diesel::insert_into(service::table)
            .values(&service.into_inner())
            .get_result::<Service>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/services")]
pub async fn list(mut db: Connection<MyDatabase>) -> ApiResponse {
    let response =
        service::table
            .select(service::all_columns)
            .load::<Service>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/service/<id>")]
pub async fn read(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response =
        service::table
            .find(id)
            .first::<Service>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[patch("/service/<id>", format = "application/json", data = "<service>")]
pub async fn update(mut db: Connection<MyDatabase>, service: Json<UpdateService>, id: i32) -> ApiResponse {
    let response =
        diesel::update(service::table.find(id))
            .set(&service.into_inner())
            .execute(&mut db)
            .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"update": count})),
        Err(err) => ApiResponse::err(err),
    }
}

#[delete("/service/<id>")]
pub async fn delete(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response
        = diesel::delete(service::table.find(id))
        .execute(&mut db)
        .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"delete": count})),
        Err(err) => ApiResponse::err(err),
    }
}
