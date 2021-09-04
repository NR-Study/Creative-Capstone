%% Robotics MATLAB
% Author: Won Bin Choi (B4)
% E-mail: dc07650@naver.com
% Organization: Sogang University(Korea, Republic of), Mechanical Engineering
% Date: September 2, 2021
close all; clear; clc;

%% Variable Initilization
Tfinal = 2;     % Time range (sec)
T = 0.001;      % Sampling time (sec)
t = 0:T:Tfinal; % Time
N = length(t);  % Number of data
% Physical Properties
L = 0.65; % Length of arm
M = 0.0;  % Mass of arm
m = 2.0;  % Mass of end effector
g = 9.81; % Gravitational acceleration
% Generalized Coordinates
ddth = zeros(N,1);  % Angular Acceleration
dth = zeros(N,1);   % Angular Velocity
th = zeros(N,1);    % Angular Position
% Initial Conditions
th(1) = 0;

%% Force Generation
tau = zeros(N,1);
for k = 1:N
%     tau(k) = cos((2*pi/1) * T * k); % Force Function
    tau(k) = 0; % Natural System
%     tau(k) = m * g * L; % Gravity Compensation
end

%% Inverse Dynamics
for k = 1:N-1
    ddth(k) = (tau(k) - (M/2 + m) * g * L * cos(th(k))) / ((M/3 + m)*power(L,2));
    dth(k+1) = dth(k) + T * ddth(k);    % 1 Step - Forward Linear Integration
    th(k+1) = th(k) + T * dth(k);
end

%% Forward Kinematics
x=zeros(N,2); y=zeros(N,2); % Initialization
for k=1:N
    x(k,1) = 0;
    y(k,1) = 0;
    x(k,2) =  L * cos(th(k,1));
    y(k,2) =  L * sin(th(k,1));
end

%% Simulator
disp("Sampling Time: " + T + " seconds, Sampling Frequency " + 1/T + " Hz");
disp("Run time: " + Tfinal + " seconds");
figure('color','w');
frame = 10;
for k = 1:frame:N
    plot(x(k,1),y(k,1),'ko'); hold on; % Joint 1
    plot(x(k,2),y(k,2),'ko'); hold on; % Joint 2
    plot([x(k,1) x(k,2)],[y(k,1) y(k,2)],'b','linewidth',2); hold on; % Link 1
    axis([-1 1 -1 1])                         % Axis X, Y
    set(gca,'DataAspectRatio',[1 1 1])
    grid on;
    drawnow;
    hold off;
end
figure('color','w');
subplot(211); % Graphs of end point
plot(t,tau,'b','linewidth',2); hold on;
legend('\tau')
ylabel('Torque(N-m)'); xlabel('time(sec)')
subplot(212); % Graphs of joint angle
plot(t,th(:,1)*180/pi,'r','linewidth',2); hold on;
legend('\theta')
ylabel('Angle(deg)'); xlabel('time(sec)')
figure('color','w');
subplot(311); % Graphs of end point
plot(t,x(:,2),'g','linewidth',2); hold on;
ylabel('x3(m)'); xlabel('time(sec)')
subplot(312); % Graphs of joint angle
plot(t,y(:,2),'r','linewidth',2); hold on;
ylabel('y3(m)'); xlabel('time(sec)')
subplot(313); % Graphs of joint angle
plot(x(:,2),y(:,2),'b','linewidth',2); hold on;
ylabel('y3(m)'); xlabel('x3(m)')