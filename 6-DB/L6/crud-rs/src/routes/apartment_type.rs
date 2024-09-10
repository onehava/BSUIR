use rocket::*;
use rocket::serde::json::Json;
use rocket::serde::json::serde_json::json;
use rocket_db_pools::Connection;
use rocket_db_pools::diesel::prelude::*;

use crate::MyDatabase;
use crud_rs::ApiResponse;

use crate::schema::apartment_type;
use crate::models::apartment_type::{ApartmentType, NewApartmentType, UpdateApartmentType};

#[post("/apartment_type", format = "application/json", data = "<apartment_type>")]
pub async fn create(mut db: Connection<MyDatabase>, apartment_type: Json<NewApartmentType>) -> ApiResponse {
    let response =
        diesel::insert_into(apartment_type::table)
            .values(&apartment_type.into_inner())
            .get_result::<ApartmentType>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/apartment_types")]
pub async fn list(mut db: Connection<MyDatabase>) -> ApiResponse {
    let response =
        apartment_type::table
            .select(apartment_type::all_columns)
            .load::<ApartmentType>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/apartment_type/<id>")]
pub async fn read(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response =
        apartment_type::table
            .find(id)
            .first::<ApartmentType>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[patch("/apartment_type/<id>", format = "application/json", data = "<apartment_type>")]
pub async fn update(mut db: Connection<MyDatabase>, apartment_type: Json<UpdateApartmentType>, id: i32) -> ApiResponse {
    let response =
        diesel::update(apartment_type::table.find(id))
            .set(&apartment_type.into_inner())
            .execute(&mut db)
            .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"update": count})),
        Err(err) => ApiResponse::err(err),
    }
}

#[delete("/apartment_type/<id>")]
pub async fn delete(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response
        = diesel::delete(apartment_type::table.find(id))
        .execute(&mut db)
        .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"delete": count})),
        Err(err) => ApiResponse::err(err),
    }
}