struct Point3 {
    pub x: i64,
    pub y: i64,
    pub z: i64,
}

fn parse( line : &str ) -> () {
    lazy_static! {
        // p=<-833,-499,-1391>, v=<84,17,61>, a=<-4,1,1>
        static ref RE_SND: Regex = Regex::new(r"^snd (\w+)$").unwrap();         // snd X
        static ref RE_SET: Regex = Regex::new(r"^set (\w+) (\S+)$").unwrap();   // set X Y
    }
    for cap in RE_SND.captures_iter( line ) {
        let value = cap[1].to_string();
        return Box::new(move |r, _mi, mo| {
            mo.push(get_v(&value, r));
            1
        })
    }
}

pub fn task12(data : &str) -> ( i32, i32 ) {
    // data.lines();
    (1,-1)
}