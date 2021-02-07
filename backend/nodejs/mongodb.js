

function connect(mongoose) {
    // Define Car model
    CarSchema = new Schema({
      brand        : String,
      type : String
    });
    mongoose.model('Car', CarSchema);
}

module.exports.make = make;