use rocket::*;
use rocket::serde::json::Json;
use rocket::serde::json::serde_json::json;
use rocket_db_pools::Connection;
use rocket_db_pools::diesel::prelude::*;

use crate::MyDatabase;
use crud_rs::ApiResponse;

use crate::schema::apartment_reservation;
use crate::models::apartment_reservation::{ApartmentReservation, NewApartmentReservation, UpdateApartmentReservation};

#[post("/apartment_reservation", format = "application/json", data = "<apartment_reservation>")]
pub async fn create(mut db: Connection<MyDatabase>, apartment_reservation: Json<NewApartmentReservation>) -> ApiResponse {
    let response =
        diesel::insert_into(apartment_reservation::table)
            .values(&apartment_reservation.into_inner())
            .get_result::<ApartmentReservation>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/apartment_reservations")]
pub async fn list(mut db: Connection<MyDatabase>) -> ApiResponse {
    let response =
        apartment_reservation::table
            .select(apartment_reservation::all_columns)
            .load::<ApartmentReservation>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/apartment_reservation/<id>")]
pub async fn read(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response =
        apartment_reservation::table
            .find(id)
            .first::<ApartmentReservation>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[patch("/apartment_reservation/<id>", format = "application/json", data = "<apartment_reservation>")]
pub async fn update(mut db: Connection<MyDatabase>, apartment_reservation: Json<UpdateApartmentReservation>, id: i32) -> ApiResponse {
    let response =
        diesel::update(apartment_reservation::table.find(id))
            .set(&apartment_reservation.into_inner())
            .execute(&mut db)
            .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"update": count})),
        Err(err) => ApiResponse::err(err),
    }
}

#[delete("/apartment_reservation/<id>")]
pub async fn delete(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response
        = diesel::delete(apartment_reservation::table.find(id))
        .execute(&mut db)
        .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"delete": count})),
        Err(err) => ApiResponse::err(err),
    }
}