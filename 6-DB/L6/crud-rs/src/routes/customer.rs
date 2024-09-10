use rocket::*;
use rocket::serde::json::Json;
use rocket::serde::json::serde_json::json;
use rocket_db_pools::Connection;
use rocket_db_pools::diesel::prelude::*;

use crate::MyDatabase;
use crud_rs::ApiResponse;

use crate::schema::customer;
use crate::models::customer::{Customer, NewCustomer, UpdateCustomer};

#[post("/customer", format = "application/json", data = "<customer>")]
pub async fn create(mut db: Connection<MyDatabase>, customer: Json<NewCustomer>) -> ApiResponse {
    let response =
        diesel::insert_into(customer::table)
            .values(&customer.into_inner())
            .get_result::<Customer>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/customers")]
pub async fn list(mut db: Connection<MyDatabase>) -> ApiResponse {
    let response =
        customer::table
            .select(customer::all_columns)
            .load::<Customer>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/customer/<id>")]
pub async fn read(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response =
        customer::table
            .find(id)
            .first::<Customer>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[patch("/customer/<id>", format = "application/json", data = "<customer>")]
pub async fn update(mut db: Connection<MyDatabase>, customer: Json<UpdateCustomer>, id: i32) -> ApiResponse {
    let response =
        diesel::update(customer::table.find(id))
            .set(&customer.into_inner())
            .execute(&mut db)
            .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"update": count})),
        Err(err) => ApiResponse::err(err),
    }
}

#[delete("/customer/<id>")]
pub async fn delete(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response
        = diesel::delete(customer::table.find(id))
        .execute(&mut db)
        .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"delete": count})),
        Err(err) => ApiResponse::err(err),
    }
}