import os,datetime
from flask import Flask,render_template,redirect,request,jsonify
app=Flask(__name__)
option=time=lhum=ltem=0

ntime=datetime.datetime.now()
@app.route('/')
def door():
    #동적 html코드
    return render_template('pr.html')
@app.route('/c',methods=['POST'])
def command():
    global option,time
    #html에서 button값을 받음
    option=request.form['button']
    #예약 모드면 예약 모드 제거
    time=0
    #'/'로 가서 pr.html다시 렌더링
    return redirect('/')
@app.route('/t',methods=['POST'])
def timecommand():
    #stime 시작시간 etime 종료시간
    global stime,etime,time
    #예약 모드시 시간값을 받음
    time=request.form['time']
    shour=int(request.form['shour'])
    smin=int(request.form['smin'])
    ssec=int(request.form['ssec'])
    ehour=int(request.form['ehour'])
    emin=int(request.form['emin'])
    esec=int(request.form['esec'])
    #연산이 가능하게 시간 형식 표준화 시작시간, 종료시간을
    #당일 0시 0분 0초로 초기화
    stime=datetime.date.strftime(ntime,'%Y-%m-%d')
    stime=datetime.datetime.strptime(stime, '%Y-%m-%d')
    etime=datetime.date.strftime(ntime,'%Y-%m-%d')
    etime=datetime.datetime.strptime(etime, '%Y-%m-%d')
    #입력받은 시간으로 변환
    stime+=datetime.timedelta(hours=shour,minutes=smin,seconds=ssec)
    etime+=datetime.timedelta(hours=ehour,minutes=emin,seconds=esec)
    #시작시간이 더 앞서 있으면 종료시간을 다음 날로 변환(버그 방지)
    #ex)시작시간 10시 종료시간 9시 ->종료시간 다음날 9시까지 출력
    if stime>etime:
        etime+=datetime.timedelta(days=1)
    print(stime,etime,time)
    #'/'로 가서 pr.html다시 렌더링
    return redirect('/')

@app.route('/cc',methods=['POST'])
def con_command():
    global condition,lhum,ltem,time
    condition=request.form['condition']
    if condition == 'tem':
        ltem=request.form['tem']
        
    elif condition == 'hum':
        lhum=request.form['hum']
    time=0
    ltem=int(ltem)
    lhum=int(lhum)
    print(ltem,lhum,condition,time)
    return redirect('/')

#actor MCU
@app.route('/act',methods=['POST'])
def arduino():
    global ntime,option
    #현재시간 갱신
    ntime=datetime.datetime.now()
    #예약 모드면 들어감
    if time:
        #ex)시작시간 8시이고 현재시간 9시면 켜짐
        if stime<ntime:
            option=1
        #아니면 꺼짐(아직 시작시간이 안됨)
        else:
            option=0
        #종료시간 오버하면 꺼짐
        if ntime>etime:
            option=0
########################시간 조건 경계##########################
    elif condition:
        if condition == 'tem':
            if lhum<=tem:
                option=1
            else:
                option=0
        elif condition == 'hum':
            if ltem<=hum:
                option=1
            else:
                option=0
    #옵션 값을 보내서 제어
    arduino={"option":option}
    return jsonify(arduino)
#pasrser MCU
@app.route('/htparse',methods=['POST'])
def parsercommand():
    global hum,tem,ntime
    status=request.get_json()
    hum=status['hum']
    tem=status['tem']
    ntime=datetime.date.now()
    logtime=datetime.date.strftime(ntime,'%Y-%m-%d %H:%M:%S')
    f=open('log.txt','a')
    f.write('%s %d %d'%(logtime,hum,tem))
    return 'Done'

port = os.getenv('PORT', '2443')
if __name__ == "__main__":
    app.run(host='0.0.0.0', port=int(port))

