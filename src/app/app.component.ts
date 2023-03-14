import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Component } from '@angular/core';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  apiUrl = 'http://utsapicar.somee.com/api/ldrs/';
  textButton = '';
  enabled = 0;
  data = [];
  timeLeft = 60;
  interval: any;

  httpOptions = {
    headers: new HttpHeaders({ 'Content-Type': 'application/json' }),
  };

  constructor(
    private http: HttpClient
  ) {
    this.getData(); // Llama al método para obtener los datos de la API
    this.startTimer(); // Llama al método para iniciar el timer
  }

  // Método para obtener los datos de la API
  getData() {
    this.http.get<any>(this.apiUrl).subscribe(data => {
      this.data = data;
      this.enabled = this.data[0]['value'];
      this.textButton = this.enabled === 1 ? 'ENCENDIDO' : 'APAGADO';
    });
  }

  // Método para accionar el led (encender o apagar) mediante el botón
  ledAction(status: number) {
    let valores = {
      id: 1,
      value: status
    }

    this.http.put<any>(this.apiUrl + "1", valores, this.httpOptions).subscribe(data => {
      this.data = data;
      this.getData();
    });
  }

  startTimer() {
    this.interval = setInterval(() => {
      this.getData();

      if (this.timeLeft > 0) {
        this.timeLeft--;
      } else {
        this.timeLeft = 60;
      }
    }, 500);
  }

  pauseTimer() {
    clearInterval(this.interval);
  }

}