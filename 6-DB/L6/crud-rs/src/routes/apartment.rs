use rocket::*;
use rocket::serde::json::Json;
use rocket::serde::json::serde_json::json;
use rocket_db_pools::Connection;
use rocket_db_pools::diesel::prelude::*;

use crate::MyDatabase;
use crud_rs::ApiResponse;

use crate::schema::apartment;
use crate::models::apartment::{Apartment, NewApartment, UpdateApartment};

#[post("/apartment", format = "application/json", data = "<apartment>")]
pub async fn create(mut db: Connection<MyDatabase>, apartment: Json<NewApartment>) -> ApiResponse {
    let response =
        diesel::insert_into(apartment::table)
        .values(&apartment.into_inner())
        .get_result::<Apartment>(&mut db)
        .await;

    ApiResponse::from(response)
}

#[get("/apartments")]
pub async fn list(mut db: Connection<MyDatabase>) -> ApiResponse {
    let response =
        apartment::table
        .select(apartment::all_columns)
        .load::<Apartment>(&mut db)
        .await;

    ApiResponse::from(response)
}

#[get("/apartment/<no>")]
pub async fn read(mut db: Connection<MyDatabase>, no: i32) -> ApiResponse {
    let response =
        apartment::table
        .find(no)
        .first::<Apartment>(&mut db)
        .await;

    ApiResponse::from(response)
}

#[patch("/apartment/<no>", format = "application/json", data = "<apartment>")]
pub async fn update(mut db: Connection<MyDatabase>, apartment: Json<UpdateApartment>, no: i32) -> ApiResponse {
    let response =
        diesel::update(apartment::table.find(no))
        .set(&apartment.into_inner())
        .execute(&mut db)
        .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"update": count})),
        Err(err) => ApiResponse::err(err),
    }
}

#[delete("/apartment/<no>")]
pub async fn delete(mut db: Connection<MyDatabase>, no: i32) -> ApiResponse {
    let response
        = diesel::delete(apartment::table.find(no))
        .execute(&mut db)
        .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"delete": count})),
        Err(err) => ApiResponse::err(err),
    }
}