use rocket::*;
use rocket::serde::json::Json;
use rocket::serde::json::serde_json::json;
use rocket_db_pools::Connection;
use rocket_db_pools::diesel::prelude::*;

use crate::MyDatabase;
use crud_rs::ApiResponse;

use crate::schema::service_order;
use crate::models::service_order::{NewServiceOrder, ServiceOrder, UpdateServiceOrder};

#[post("/service_order", format = "application/json", data = "<service_order>")]
pub async fn create(mut db: Connection<MyDatabase>, service_order: Json<NewServiceOrder>) -> ApiResponse {
    let response =
        diesel::insert_into(service_order::table)
            .values(&service_order.into_inner())
            .get_result::<ServiceOrder>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/service_orders")]
pub async fn list(mut db: Connection<MyDatabase>) -> ApiResponse {
    let response =
        service_order::table
            .select(service_order::all_columns)
            .load::<ServiceOrder>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/service_order/<guest_id>/<service_id>")]
pub async fn read(mut db: Connection<MyDatabase>, guest_id: i32, service_id: i32) -> ApiResponse {
    let response =
        service_order::table
            .find((guest_id, service_id))
            .first::<ServiceOrder>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[patch("/service_order/<guest_id>/<service_id>", format = "application/json", data = "<service>")]
pub async fn update(mut db: Connection<MyDatabase>, service: Json<UpdateServiceOrder>, guest_id: i32, service_id: i32) -> ApiResponse {
    let response =
        diesel::update(service_order::table.find((guest_id, service_id)))
            .set(&service.into_inner())
            .execute(&mut db)
            .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"update": count})),
        Err(err) => ApiResponse::err(err),
    }
}

#[delete("/service/<guest_id>/<service_id>")]
pub async fn delete(mut db: Connection<MyDatabase>, guest_id: i32, service_id: i32) -> ApiResponse {
    let response
        = diesel::delete(service_order::table.find((guest_id, service_id)))
        .execute(&mut db)
        .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"delete": count})),
        Err(err) => ApiResponse::err(err),
    }
}
