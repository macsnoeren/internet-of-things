const crypto = require('crypto');

data = "IGc6eUfF6B0OSPrp3w66KNrrJ0OI0YfOvHuqH1uamE7F3hybzOvbQEzlta/hpcLKrHu0BouHHh4oZmeDc7pW1YthzTsNzpmy4+0vfT51LSzBuRV5q0q+xocW6x8KieJ7tELzMbzLd2HUPsEhR8PX/DV5qTUJtAik9bMLBkXoNr4=HxIm3uQsEaq6wa93l//BKevntcaVD/YX7mxOp+GiZgk=";
key = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

ciphertext_b64 = data.slice(0, data.length-44);
iv_b64         = data.slice(data.length-44, data.length);

iv_buffer = Buffer.from(iv_b64, 'base64');
iv        = iv_buffer.toString('ascii').slice(0, 16); // Should be 16 big!

let decypher = crypto.createDecipheriv('aes-256-cbc', key, iv);
let decrypted = decypher.update(data, 'base64', 'ascii');

console.log(decrypted);