// @generated automatically by Diesel CLI.

diesel::table! {
    apartment (no) {
        no -> Int4,
        free -> Bool,
        type_id -> Int4,
    }
}

diesel::table! {
    apartment_reservation (id) {
        id -> Int4,
        date -> Date,
        paid -> Bool,
        check_in -> Date,
        check_out -> Date,
        apartment_no -> Int4,
        customer_id -> Int4,
    }
}

diesel::table! {
    apartment_type (id) {
        id -> Int4,
        #[max_length = 64]
        name -> Varchar,
        description -> Nullable<Text>,
        capacity -> Int4,
        cost -> Int4,
    }
}

diesel::table! {
    company_identity (id) {
        id -> Int4,
        #[max_length = 64]
        name -> Varchar,
        #[max_length = 9]
        taxpayer_id -> Varchar,
    }
}

diesel::table! {
    customer (id) {
        id -> Int4,
        individual_id -> Nullable<Int4>,
        company_id -> Nullable<Int4>,
    }
}

diesel::table! {
    guest (id) {
        id -> Int4,
        individual_id -> Int4,
        reservation_id -> Int4,
    }
}

diesel::table! {
    individual_identity (id) {
        id -> Int4,
        #[max_length = 64]
        name -> Varchar,
        #[max_length = 14]
        passport_id -> Varchar,
    }
}

diesel::table! {
    service (id) {
        id -> Int4,
        #[max_length = 64]
        name -> Varchar,
        description -> Nullable<Text>,
        cost -> Int4,
    }
}

diesel::table! {
    service_order (guest_id, service_id) {
        guest_id -> Int4,
        service_id -> Int4,
        order_date -> Date,
        service_date -> Date,
        paid -> Bool,
    }
}

diesel::joinable!(apartment -> apartment_type (type_id));
diesel::joinable!(apartment_reservation -> apartment (apartment_no));
diesel::joinable!(apartment_reservation -> customer (customer_id));
diesel::joinable!(customer -> company_identity (company_id));
diesel::joinable!(customer -> individual_identity (individual_id));
diesel::joinable!(guest -> apartment_reservation (reservation_id));
diesel::joinable!(guest -> individual_identity (individual_id));
diesel::joinable!(service_order -> guest (guest_id));
diesel::joinable!(service_order -> service (service_id));

diesel::allow_tables_to_appear_in_same_query!(
    apartment,
    apartment_reservation,
    apartment_type,
    company_identity,
    customer,
    guest,
    individual_identity,
    service,
    service_order,
);
