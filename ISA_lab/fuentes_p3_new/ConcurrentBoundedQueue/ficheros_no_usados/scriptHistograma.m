clc; close all; clear all;
cd 'C:\Users\emage\Google Drive (650375@unizar.es)\Universidad\MATERIAL CLASE\TFG\parte2cola\fuentes_p3_new\ConcurrentBoundedQueue';

%%
fid = fopen('histograma3.txt');
T = textscan(fid, '%u');
%histogram(T{1,1}(:), 10);
histograma = T{1,1}(:);
figure;
plot(histograma);
xlim([0 length(histograma)]);