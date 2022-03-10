# Student-Info-Enrollment-System


**Programın Açılışı**

Program açıldığında kullanıcıdan tarih, maksimum kredi ve maksimum ders sayısı alınır.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.004.png)

**Programın Fonksiyonları ve Ekran Çıktıları**

**void Lecture\_travel**

Ekrana LECTURE linkli listesindeki düğümlerin özelliklerini basar. Ekran çıktısı load\_lectures fonksiyon açıklamasında gösterilecek.

**void Student\_travel**

Ekrana STUDENT linkli listesindeki düğümlerin özelliklerini basar. Ekran çıktısı load\_students fonksiyon açıklamasında gösterilecek.

**void load\_lectures**

Kullanıcıdan ilk verileri aldıktan sonra while döngüsü içinde çağırılır. (

Temel de yaptığı iş program çalıştığında *dersler.txt* dosyasını okuma moduna açar ve oradaki dersleri LECTURE linkli listesine ekler.  load\_lectures her ders için tekrar çağırılır. (189.satır)

Programı çalıştırıp load\_lectures ve Lecture\_travel fonksiyonlarının çalıştığını görelim.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.005.png)

**void load\_students** 

load\_lectures ile aynı yapıda çalışır. Tek farkı ogrenciler.txt dosyasındaki verilere STUDENT linkli listesine atar.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.006.png)

**void enroll**

**Enroll** fonksiyonu gönderdiğimiz *mode* parametresine iki farklı şekilde çalışır.

**mode=1** için : kullanıcının ders kaydı yapmasını sağlar.  Kullanıcın girdiği kredi ve ders limiti fonksiyona parameter olarak gönderilir ki kayıt koşullarının sağlıyor mu diye baksın.

Kullanıcı ders kaydı yaptığında öğrencinin kredisi(credit) ve aldığı ders sayısı(lectureNumber) güncellenir, dersin de kontenjan bilgisi(quota) ve dersi alan toplam kişi bilgisi(counter) güncellenir.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.007.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.008.png)![]/png/(Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.009.png)

Yukarıdaki inputlara göre Eddard Stark öğrencisine iki adet ders eklemeye çalışalım. İlk ders iki kredilik, olsun. Beklentimiz: ilk dersi ekleyebilecek fakat ikinci dersi hem kredi limitini hem de ders sayısını aşacağı için ekleyemeycek.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.010.png)


Birinci işem sonucunda ogrenciDersKayit.txt dosyamız da güncellenecektir.(enroll’un içinden log\_update fonksiyonu çağrılır ve bu dosyayı düzenler). 




![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.008.png)![](Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.011.png)

STUDENT ve LECTURE linkli listesinin ilgili node’larının gerekli yerlerinde(dersin kontenjanında, öğrencinin krediside vs.) değişiklikler yapılır. Ancak kullanıcı yine program içinde bunları değiştireceği için bu değişiklikler dosyaya çıkış menüsüne basıldığında yapılır (-1).

Şimdi de ikinci dersi eklemeye çalışalım.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.012.png)

Beklediğimiz gibi kredi limitini aştığımız için ekleyemedik.


Diğer uç durumları inceleyelim

Hatalı ders kodu veya öğrenci ID’si grime durumu:

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.013.png) ![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.014.png)

Zaten kayıtlı olan bir öğrencinin tekrar dersi almaya çalışması durumu:

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.015.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.016.png)

Diğer bir uç durum da dersin kontenjanının dolduğu durum. Bu durumu manuel ders açma (open\_lecture) fonksiyonundan sonra düşük kontenjanlı bir ders açarak göstereceğim.

**mode=2 :** El ile ders eklemeden tek farkı programın başında çağrılmasıdır.OgrenciDersKayit.txt inputunu okuyarak gerekli kayır işlemlerini yaparlar. Mode 1 ve 2 ‘yi ayıran nokta enroll fonksitonun başındaki bir if bloğudur.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.017.png) ![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.018.png)






**void open\_lecture\_manuel**

LECTURE linkli listesinin başının pointerı fonksiyona gönderilir. Gerekli bilgiler alınır. Code bilgisine göre linkli listede uygun yere konur.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.019.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.020.png)![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.021.png)

Kontenjanı 2 olarak belirlemenin sebebi kontenjanın aşılamayacağını göstermek.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.022.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.023.png)

Artık kontenjanımız doldu. Öğrenci ekleyemememiz lazım.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.024.png)

Görüldüğü gibi program dersi eklemedi.

Aynı dersi bir daha açmaya izin vermemeli.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.025.png)








Şimdi yapılan değişiklikleri dosyaya işlemek için -1’e basarak çıkış yapalım ve dosyaya bakalım

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.020.png)![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.026.png)

**void new\_student**

Aynı ***void open\_lecture\_manuel*** gibi çalışır ancak bu çift yönlü bir linkli listedir. Öğrenci numarasına göre uygun yere yerleştirilir. Bu fonksiyon da var olan bir öğrenciyi tekrar listeye eklemez.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.027.png)


Yine eklemeyi deneyelim.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.028.png).

İzin vermedi.

Şimdi programı ana menüden -1’e basarak kapatalım ve güncellenecek olan dosyalara bakalım.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.029.png)

**void delete\_student**

**void close\_lecture** fonksiyonun tam tersi şekilde çalışır. Yine ID alınır ve STUDENT listesinden bu öğrenci node’unu bulur ve öğrencinin aldığı derslere bakıp LECTURE listesinde gezinip o dersleri bulduktan derse kayıtlı olan öğrenciler listesinden öğrenciyi siler. Dersin boş kontenjanı 1 arttırır. 

ogrenciler.txt dosyasından öğrenci silinir.




![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.030.png)![](Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.031.png)

Programdan menü aracılığıyla çıkış yaptığımızda dosyada ad değişikliği göreceğiz.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.030.png)![](Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.032.png)


Ayrıca olmayan bir öğrenciyi silemeyiz. 111111 numarasını silmeyi deneyelim

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.033.png)

**void drop**

**enroll** fonksiyonun tersidir. Öğrenciyi dersten silmek için kullanılır. Ders kodu ve öğrenci ID’si istenir. Önce dersin node’u sonra öğrencinin node’u bulunur. Dersi alan öğrencilerden öğrenci silinir, öğrencinin aldığı derslerden bu ders silinir. Öğrenciye kredisi ve ders alma sayısı iade edilir. Dersi alan kişi sayısı bir azaltılır, kontenjanda 1 yer açılır.

Ogrenciler.txt dosyasında öğrencinin kredi ve ders bilgisi güncellenir, ogrenciDersKayit.txt dosyasında kayitli olan log sildi olarak değiştirilir.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.034.png)

Görüldüğü gibi 22011010 numaralı öğrencinin aldığı dersler artık boş, eskiden BLM2222 dersi vardı.

Şimdi bir de ogrenciDersKayit.txt’ye bakalım.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.035.png)![](Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.036.png)

Görüldüğü gibi log güncellendi.

**void Lecture\_Write**

Bu fonksiyon , ismi kullanıcının girdiği ders kodu olan (örn: BLM1012.txt) bir dosya oluştururve bu klasöre o dersi alan öğrencileri yazar.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.037.png)

Fonksiyonu çağırmadan önce.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.038.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.039.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.040.png)

Fonksiyonu çağırdıktan sonra.

**void Students\_Write**

Yukardaki fonksiyona benzer şekilde öğrencinin aldığı derslerin alfabetik sıralandığı bir dosya oluşturur.(örn:22011001.txt)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.041.png) Fonksiyonu çağırmadan önce

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.042.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.043.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.044.png)

Çağırdıktan sonra.

**void close\_lecture**

Bu fonksiyon LECTURE listesinin başını parametre olarak alır ve kullanıcıdan kapatılacak dersin kodunu ister. Kod girildikten sonra o dersi bulana kadar liste üzerinde dolaşır, bulamazsa kullanıcıyı uyarır. Dersi bulduktan sonra o node’taki dersi alan öğrenciler dizisinden(tmp->enrolled\_students[i] derse kayıtlı öğrencilerin ID’lerine bakarak STUDENT listesinde o ID’ye sahip olan öğrencileri bulur ve öğrencinin aldığı derslerden(tmpS->lecture\_code[j]) bu dersin kodunu siler daha sonra bu öğrencilere dersin kredisi kadar krediyi iade eder ve öğrencinin alabileceği ders sayısını bir arttırır.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.045.png)

Olmayan bir dersi silemeyiz.


Şimdi BLM3130 dersini kapatalım. Burada beklentimiz log dosyasında işaretli yerin “ders kapandi” olması, dersler dosyasından BLM3130’un silinmesi ve 22011007 numaralı öğrencinin ders ve kredilerini iade almasıdır. Fonksiyon çalışmadan önce dosyalara bakalım.

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.046.png)![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.047.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.046.png)![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.048.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.049.png)![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.050.png)


Programdan ders silelim

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.051.png)

İşlem sonrasında dosyalara bakalım

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.052.png)![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.053.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.054.png)![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.055.png)

![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.056.png)![](/png/Aspose.Words.ff7bcd9c-429b-48d9-8b6a-822f8f29e35d.057.png)

