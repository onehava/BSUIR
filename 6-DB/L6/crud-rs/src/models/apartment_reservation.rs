use crate::schema::*;
use diesel::{AsChangeset, Identifiable, Insertable, Queryable};
use rocket::serde::{Deserialize, Serialize};
use chrono::NaiveDate;

#[derive(Queryable, Identifiable, Serialize, Deserialize)]
#[diesel(table_name = apartment_reservation)]
pub struct ApartmentReservation {
    pub id: i32,
    pub date: NaiveDate,
    pub paid: bool,
    pub check_in: NaiveDate,
    pub check_out: NaiveDate,
    pub apartment_no: i32,
    pub customer_id: i32,
}

#[derive(Insertable, Deserialize)]
#[diesel(table_name = apartment_reservation)]
pub struct NewApartmentReservation {
    pub date: NaiveDate,
    pub paid: bool,
    pub check_in: NaiveDate,
    pub check_out: NaiveDate,
    pub apartment_no: i32,
    pub customer_id: i32,
}

#[derive(AsChangeset, Deserialize)]
#[diesel(table_name = apartment_reservation)]
pub struct UpdateApartmentReservation {
    pub date: Option<NaiveDate>,
    pub paid: Option<bool>,
    pub check_in: Option<NaiveDate>,
    pub check_out: Option<NaiveDate>,
    pub apartment_no: Option<i32>,
    pub customer_id: Option<i32>,
}