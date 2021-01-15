# XMLread easy util

XMLread - command line console util reads XML file (stream), parses, decode text into console locale and shows as a tree.
I can be useful for manual checking xml-files for integrity and content.

### Input XML examle

```xml
<?xml version="1.0" encoding="utf-8"?>
 <soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <soap:Body>
   <ProcessPayment xmlns="http://usmp.com.ua/">
   <request>
   <Serial>51G34CYS37ZV9BAD32143</Serial>
   <KeyWord>qwerty</KeyWord>
   <Payments xmlns="http://www.unity.net/">
   <PaymentDetails>
   <Date>2008-05-28T16:21:16.859375+03:00</Date>
   <PayElementID>36</PayElementID>
   <Account>(067)0000000</Account>
   <Amount>20000</Amount>
   <ChequeNumber>4</ChequeNumber>
   </PaymentDetails>
   </Payments>
   </request>
  </ProcessPayment>
 </soap:Body>
</soap:Envelope>
```

### Output tree list

```text
soap:Envelope
 xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"
 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 xmlns:xsd="http://www.w3.org/2001/XMLSchema"
    |soap:Body
        |ProcessPayment
         xmlns="http://usmp.com.ua/"
            |request
                |Serial = 51G34CYS37ZV9BAD32143
                |KeyWord = qwerty
                |Payments
                 xmlns="http://www.unity.net/"
                    |PaymentDetails
                        |Date = 2008-05-28T16:21:16.859375+03:00
                        |PayElementID = 36
                        |Account = (067)0000000
                        |Amount = 20000
                        |ChequeNumber = 4
```

## Build Util

To build util you need the devel pacjake of libxml2 lib installed. To build util just run next commands:

```sh
$ make clean && make
$ sudo cp xmlread /usr/local/bin/
```
