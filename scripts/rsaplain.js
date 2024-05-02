class RSAProvider {

    publicKey = "";
    spki = null;

    constructor(key) {
        this.publicKey = key;
    }

    base64ToArrayBuffer(base64) {
        var binary_string = window.atob(base64);
        var len = binary_string.length;
        var bytes = new Uint8Array(len);
        for (var i = 0; i < len; i++) {
            bytes[i] = binary_string.charCodeAt(i);
        }
        return bytes.buffer;
    }

    arrayBufferToBase64(buffer) {
        var binary = '';
        var bytes = new Uint8Array(buffer);
        var len = bytes.byteLength;
        for (var i = 0; i < len; i++) {
            binary += String.fromCharCode(bytes[i]);
        }
        return window.btoa(binary);
    }

    initialize() {
        const binaryDer = this.base64ToArrayBuffer(this.publicKey);
        return window.crypto.subtle.importKey(
            "spki",
            binaryDer, {
                name: 'RSA-OAEP',
                modulusLength: 256,
                hash: {
                    name: 'sha-256'
                }
            },
            false,
            ["encrypt"]
        ).then(res => {
            this.spki = res;          
        });   
    }

    encrypt(message) {
        let spki = this.spki;
        let enc = new TextEncoder();
        let encodedMessage = enc.encode(message);      

        return window.crypto.subtle.encrypt(
            {
                name: "RSA-OAEP"
            },
            spki,
            encodedMessage
        ).then(res => {
            return this.arrayBufferToBase64(res);
        });
    }
}
