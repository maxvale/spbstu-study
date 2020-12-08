package com.example.client;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.apache.http.conn.ssl.NoopHostnameVerifier;
import org.apache.http.conn.ssl.SSLConnectionSocketFactory;
import org.apache.http.conn.ssl.TrustSelfSignedStrategy;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.ssl.SSLContextBuilder;

import org.springframework.context.annotation.Bean;
import org.springframework.core.io.ClassPathResource;
import org.springframework.http.client.HttpComponentsClientHttpRequestFactory;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestTemplate;
import org.springframework.core.env.Environment;

import java.io.InputStream;
import java.net.URI;
import java.net.http.HttpClient;
import java.security.KeyStore;
import java.util.Objects;

@SpringBootApplication
public class ClientApplication {

	public static void main(String[] args) {
		SpringApplication.run(ClientApplication.class, args);
	}

	@Bean
	public RestTemplate getRestTemplate() {
		RestTemplate restTemplate = new RestTemplate();
		KeyStore keyStore;
		HttpComponentsClientHttpRequestFactory requestFactory = null;
		try {
			keyStore = KeyStore.getInstance("jks");
			ClassPathResource classPathResource = new
					ClassPathResource("gateway.jks");
			InputStream inputStream = classPathResource.getInputStream();
			keyStore.load(inputStream, "vale9519".toCharArray());
			SSLConnectionSocketFactory socketFactory = new
					SSLConnectionSocketFactory(new SSLContextBuilder()
					.loadTrustMaterial(null, new TrustSelfSignedStrategy())
					.loadKeyMaterial(keyStore, "vale9519".toCharArray()).build(),
					NoopHostnameVerifier.INSTANCE);
			CloseableHttpClient httpClient =
					HttpClients.custom().setSSLSocketFactory(socketFactory)
							.setMaxConnTotal(5)
							.setMaxConnPerRoute(5)
							.build();
			requestFactory = new HttpComponentsClientHttpRequestFactory(httpClient);
			requestFactory.setReadTimeout(10000);
			requestFactory.setConnectTimeout(10000);
			restTemplate.setRequestFactory(requestFactory);
		} catch (Exception exception) {
			exception.printStackTrace();
		}
		return restTemplate;
	}

	@RestController
	@RequestMapping(value="/gateway")
	public class GatewayController {
		@Autowired
		RestTemplate restTemplate;
		@Autowired
		Environment env;

		@RequestMapping(value = "/data", method = RequestMethod.GET)
		public String getData() {
			System.out.println("Returning data from gateway");
			return "Hello from gateway";
		}
		@RequestMapping(value = "/server-data", method = RequestMethod.GET)
		public String getServerData() {
			System.out.println("Returning data from serer through gateway");
			try {
				String msEndpoint = env.getProperty("endpoint.server");
				System.out.println("Endpoint name : [" + msEndpoint + "]");
				return restTemplate.getForObject(new
						URI(Objects.requireNonNull(msEndpoint)), String.class);
			} catch (Exception ex) {
				ex.printStackTrace();
			}
			return "Exception occurred";
		}
	}

}
