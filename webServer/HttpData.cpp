// @Author Lin Ya
// @Email xxbbb@vip.qq.com
#include "HttpData.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <iostream>
#include "Channel.h"
#include "EventLoop.h"
#include "Util.h"
#include "time.h"
#include <iostream>
#include<sys/uio.h>
#include <unistd.h>

//todo 测试使用
static int test_index=0;


using namespace std;

pthread_once_t MimeType::once_control = PTHREAD_ONCE_INIT;
std::unordered_map<std::string, std::string> MimeType::mime;

const __uint32_t DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const int DEFAULT_EXPIRED_TIME = 2000;              // ms
const int DEFAULT_KEEP_ALIVE_TIME = 5 * 60 * 1000;  // ms

char favicon[555] = {
    '\x89', 'P',    'N',    'G',    '\xD',  '\xA',  '\x1A', '\xA',  '\x0',
    '\x0',  '\x0',  '\xD',  'I',    'H',    'D',    'R',    '\x0',  '\x0',
    '\x0',  '\x10', '\x0',  '\x0',  '\x0',  '\x10', '\x8',  '\x6',  '\x0',
    '\x0',  '\x0',  '\x1F', '\xF3', '\xFF', 'a',    '\x0',  '\x0',  '\x0',
    '\x19', 't',    'E',    'X',    't',    'S',    'o',    'f',    't',
    'w',    'a',    'r',    'e',    '\x0',  'A',    'd',    'o',    'b',
    'e',    '\x20', 'I',    'm',    'a',    'g',    'e',    'R',    'e',
    'a',    'd',    'y',    'q',    '\xC9', 'e',    '\x3C', '\x0',  '\x0',
    '\x1',  '\xCD', 'I',    'D',    'A',    'T',    'x',    '\xDA', '\x94',
    '\x93', '9',    'H',    '\x3',  'A',    '\x14', '\x86', '\xFF', '\x5D',
    'b',    '\xA7', '\x4',  'R',    '\xC4', 'm',    '\x22', '\x1E', '\xA0',
    'F',    '\x24', '\x8',  '\x16', '\x16', 'v',    '\xA',  '6',    '\xBA',
    'J',    '\x9A', '\x80', '\x8',  'A',    '\xB4', 'q',    '\x85', 'X',
    '\x89', 'G',    '\xB0', 'I',    '\xA9', 'Q',    '\x24', '\xCD', '\xA6',
    '\x8',  '\xA4', 'H',    'c',    '\x91', 'B',    '\xB',  '\xAF', 'V',
    '\xC1', 'F',    '\xB4', '\x15', '\xCF', '\x22', 'X',    '\x98', '\xB',
    'T',    'H',    '\x8A', 'd',    '\x93', '\x8D', '\xFB', 'F',    'g',
    '\xC9', '\x1A', '\x14', '\x7D', '\xF0', 'f',    'v',    'f',    '\xDF',
    '\x7C', '\xEF', '\xE7', 'g',    'F',    '\xA8', '\xD5', 'j',    'H',
    '\x24', '\x12', '\x2A', '\x0',  '\x5',  '\xBF', 'G',    '\xD4', '\xEF',
    '\xF7', '\x2F', '6',    '\xEC', '\x12', '\x20', '\x1E', '\x8F', '\xD7',
    '\xAA', '\xD5', '\xEA', '\xAF', 'I',    '5',    'F',    '\xAA', 'T',
    '\x5F', '\x9F', '\x22', 'A',    '\x2A', '\x95', '\xA',  '\x83', '\xE5',
    'r',    '9',    'd',    '\xB3', 'Y',    '\x96', '\x99', 'L',    '\x6',
    '\xE9', 't',    '\x9A', '\x25', '\x85', '\x2C', '\xCB', 'T',    '\xA7',
    '\xC4', 'b',    '1',    '\xB5', '\x5E', '\x0',  '\x3',  'h',    '\x9A',
    '\xC6', '\x16', '\x82', '\x20', 'X',    'R',    '\x14', 'E',    '6',
    'S',    '\x94', '\xCB', 'e',    'x',    '\xBD', '\x5E', '\xAA', 'U',
    'T',    '\x23', 'L',    '\xC0', '\xE0', '\xE2', '\xC1', '\x8F', '\x0',
    '\x9E', '\xBC', '\x9',  'A',    '\x7C', '\x3E', '\x1F', '\x83', 'D',
    '\x22', '\x11', '\xD5', 'T',    '\x40', '\x3F', '8',    '\x80', 'w',
    '\xE5', '3',    '\x7',  '\xB8', '\x5C', '\x2E', 'H',    '\x92', '\x4',
    '\x87', '\xC3', '\x81', '\x40', '\x20', '\x40', 'g',    '\x98', '\xE9',
    '6',    '\x1A', '\xA6', 'g',    '\x15', '\x4',  '\xE3', '\xD7', '\xC8',
    '\xBD', '\x15', '\xE1', 'i',    '\xB7', 'C',    '\xAB', '\xEA', 'x',
    '\x2F', 'j',    'X',    '\x92', '\xBB', '\x18', '\x20', '\x9F', '\xCF',
    '3',    '\xC3', '\xB8', '\xE9', 'N',    '\xA7', '\xD3', 'l',    'J',
    '\x0',  'i',    '6',    '\x7C', '\x8E', '\xE1', '\xFE', 'V',    '\x84',
    '\xE7', '\x3C', '\x9F', 'r',    '\x2B', '\x3A', 'B',    '\x7B', '7',
    'f',    'w',    '\xAE', '\x8E', '\xE',  '\xF3', '\xBD', 'R',    '\xA9',
    'd',    '\x2',  'B',    '\xAF', '\x85', '2',    'f',    'F',    '\xBA',
    '\xC',  '\xD9', '\x9F', '\x1D', '\x9A', 'l',    '\x22', '\xE6', '\xC7',
    '\x3A', '\x2C', '\x80', '\xEF', '\xC1', '\x15', '\x90', '\x7',  '\x93',
    '\xA2', '\x28', '\xA0', 'S',    'j',    '\xB1', '\xB8', '\xDF', '\x29',
    '5',    'C',    '\xE',  '\x3F', 'X',    '\xFC', '\x98', '\xDA', 'y',
    'j',    'P',    '\x40', '\x0',  '\x87', '\xAE', '\x1B', '\x17', 'B',
    '\xB4', '\x3A', '\x3F', '\xBE', 'y',    '\xC7', '\xA',  '\x26', '\xB6',
    '\xEE', '\xD9', '\x9A', '\x60', '\x14', '\x93', '\xDB', '\x8F', '\xD',
    '\xA',  '\x2E', '\xE9', '\x23', '\x95', '\x29', 'X',    '\x0',  '\x27',
    '\xEB', 'n',    'V',    'p',    '\xBC', '\xD6', '\xCB', '\xD6', 'G',
    '\xAB', '\x3D', 'l',    '\x7D', '\xB8', '\xD2', '\xDD', '\xA0', '\x60',
    '\x83', '\xBA', '\xEF', '\x5F', '\xA4', '\xEA', '\xCC', '\x2',  'N',
    '\xAE', '\x5E', 'p',    '\x1A', '\xEC', '\xB3', '\x40', '9',    '\xAC',
    '\xFE', '\xF2', '\x91', '\x89', 'g',    '\x91', '\x85', '\x21', '\xA8',
    '\x87', '\xB7', 'X',    '\x7E', '\x7E', '\x85', '\xBB', '\xCD', 'N',
    'N',    'b',    't',    '\x40', '\xFA', '\x93', '\x89', '\xEC', '\x1E',
    '\xEC', '\x86', '\x2',  'H',    '\x26', '\x93', '\xD0', 'u',    '\x1D',
    '\x7F', '\x9',  '2',    '\x95', '\xBF', '\x1F', '\xDB', '\xD7', 'c',
    '\x8A', '\x1A', '\xF7', '\x5C', '\xC1', '\xFF', '\x22', 'J',    '\xC3',
    '\x87', '\x0',  '\x3',  '\x0',  'K',    '\xBB', '\xF8', '\xD6', '\x2A',
    'v',    '\x98', 'I',    '\x0',  '\x0',  '\x0',  '\x0',  'I',    'E',
    'N',    'D',    '\xAE', 'B',    '\x60', '\x82',
};

void MimeType::init() {
  mime[".html"] = "text/html";
  mime[".avi"] = "video/x-msvideo";
  mime[".bmp"] = "image/bmp";
  mime[".c"] = "text/plain";
  mime[".doc"] = "application/msword";
  mime[".gif"] = "image/gif";
  mime[".gz"] = "application/x-gzip";
  mime[".htm"] = "text/html";
  mime[".ico"] = "image/x-icon";
  mime[".jpg"] = "image/jpeg";
  mime[".png"] = "image/png";
  mime[".txt"] = "text/plain";
  mime[".mp3"] = "audio/mp3";
  mime["default"] = "text/html";
}

std::string MimeType::getMime(const std::string &suffix) {
  pthread_once(&once_control, MimeType::init);
  if (mime.find(suffix) == mime.end())
    return mime["default"];
  else
    return mime[suffix];
}

HttpData::HttpData(EventLoop *loop, int connfd)
    : loop_(loop),
      channel_(new Channel(loop, connfd)),
      fd_(connfd),
      error_(false),
      connectionState_(H_CONNECTED),
      method_(METHOD_GET),
      HTTPVersion_(HTTP_11),
      nowReadPos_(0),
      state_(STATE_PARSE_URI),
      hState_(H_START),
      keepAlive_(false) {
  // loop_->queueInLoop(bind(&HttpData::setHandlers, this));
  channel_->setReadHandler(bind(&HttpData::handleRead, this));
  channel_->setWriteHandler(bind(&HttpData::handleWrite, this));
  channel_->setConnHandler(bind(&HttpData::handleConn, this));
}

void HttpData::reset() {
  // inBuffer_.clear();
  fileName_.clear();
  path_.clear();
  nowReadPos_ = 0;
  state_ = STATE_PARSE_URI;
  hState_ = H_START;
  headers_.clear();
  // keepAlive_ = false;
  if (timer_.lock()) {
    shared_ptr<TimerNode> my_timer(timer_.lock());
    my_timer->clearReq();
    timer_.reset();
  }
}

void HttpData::seperateTimer() {
  // cout << "seperateTimer" << endl;
  if (timer_.lock()) {
    shared_ptr<TimerNode> my_timer(timer_.lock());
    my_timer->clearReq();
    timer_.reset();
  }
}

void HttpData::handleRead() {
//handleRead：begin---->收到消息，开始读取消息进行解析------------------
  __uint32_t &events_ = channel_->getEvents();
  do {
    bool zero = false;
    int read_num = readn(fd_, inBuffer_, zero);
    LOG << "Request: " << inBuffer_;
    if (connectionState_ == H_DISCONNECTING) {
      inBuffer_.clear();
      break;
    }
    // cout << inBuffer_ << endl;
    if (read_num < 0) {
      perror("1");
      error_ = true;
      handleError(fd_, 400, "Bad Request");
      break;
    }
    else if (read_num == 0)
    {
        error_ = true;
         break;
    }
    else if (zero) {
      // 有请求出现但是读不到数据，可能是Request
      // Aborted，或者来自网络的数据没有达到等原因
      // 最可能是对端已经关闭了，统一按照对端已经关闭处理
      // error_ = true;
      connectionState_ = H_DISCONNECTING;
      if (read_num == 0) {
        // error_ = true;
        break;
      }
      // cout << "readnum == 0" << endl;
    }

    if (state_ == STATE_PARSE_URI) {
      URIState flag = this->parseURI();
      if (flag == PARSE_URI_AGAIN){
         break;
      }
      else if (flag == PARSE_URI_ERROR) {
        perror("httpdata.cpp perror_hhss 2:");
        LOG << "FD = " << fd_ << "," << inBuffer_ << "******";
        inBuffer_.clear();
        error_ = true;
        handleError(fd_, 400, "Bad Request");
        break;
      } else{
        state_ = STATE_PARSE_HEADERS;
      }
        
    }
    if (state_ == STATE_PARSE_HEADERS) {
      HeaderState flag = this->parseHeaders();
      if (flag == PARSE_HEADER_AGAIN)
        break;
      else if (flag == PARSE_HEADER_ERROR) {
        perror("3");
        error_ = true;
        handleError(fd_, 400, "Bad Request");
        break;
      }
      if (method_ == METHOD_POST) {
        // POST方法准备
        state_ = STATE_RECV_BODY;
      } else {
        state_ = STATE_ANALYSIS;
      }
    }
    if (state_ == STATE_RECV_BODY) {
    //todo 
      int content_length = -1;
      //todo 
      /*for(auto it=headers_.begin();it!=headers_.end();++it){
      
        cout<<it->first<<" : "<<it->second<<endl;
      }*/
      if (headers_.find("Content-Length") != headers_.end()) {
        content_length = atoi(headers_["Content-Length"].c_str());
        //todo

      } else {
        // cout << "(state_ == STATE_RECV_BODY)" << endl;
        error_ = true;
        handleError(fd_, 400, "Bad Request: Lack of argument (Content-length)");
        break;
      }
      if (static_cast<int>(inBuffer_.size()) < content_length) break;
      state_ = STATE_ANALYSIS;
    }
    if (state_ == STATE_ANALYSIS) {
      AnalysisState flag = this->analysisRequest();
      if (flag == ANALYSIS_SUCCESS) {
        state_ = STATE_FINISH;
        break;
      } else {
        // cout << "state_ == STATE_ANALYSIS" << endl;
        error_ = true;
        break;
      }
    }
  } while (false);
  // cout << "state_=" << state_ << endl;
  if (!error_) {
    if (outBuffer_.size() > 0) {
      handleWrite();
      //events_ |= EPOLLOUT;
    }
    // error_ may change 直到读完为止 errno=eagain
    if (!error_ && state_ == STATE_FINISH) {
      this->reset();
      if (inBuffer_.size() > 0) {
        if (connectionState_ != H_DISCONNECTING){
        handleRead();
        } 
      }

      // if ((keepAlive_ || inBuffer_.size() > 0) && connectionState_ ==
      // H_CONNECTED)
      // {
      //     this->reset();
      //     events_ |= EPOLLIN;
      // }
    } else if (!error_ && connectionState_ != H_DISCONNECTED)
      events_ |= EPOLLIN;
  }
  //handleRead：end---->消息解析完毕------------------
}

void HttpData::handleWrite() {
  if (!error_ && connectionState_ != H_DISCONNECTED) {
    __uint32_t &events_ = channel_->getEvents();

    if (writen(fd_, outBuffer_) < 0) {
      perror("writen");
      events_ = 0;
      error_ = true;
    }

    if (outBuffer_.size() > 0){
    //writen(fd_, outBuffer_);
    events_ |= EPOLLOUT;
    }else{
      events_ &= (~EPOLLOUT);
    }
  }
}

void HttpData::handleConn() {
  seperateTimer();
  __uint32_t &events_ = channel_->getEvents();
  if (!error_ && connectionState_ == H_CONNECTED) {
    if (events_ != 0) {
      int timeout = DEFAULT_EXPIRED_TIME;
      if (keepAlive_) timeout = DEFAULT_KEEP_ALIVE_TIME;
      if ((events_ & EPOLLIN) && (events_ & EPOLLOUT)) {
        events_ = __uint32_t(0);
        events_ |= EPOLLOUT;
      }
      // events_ |= (EPOLLET | EPOLLONESHOT);
      events_ |= EPOLLET;
      loop_->updatePoller(channel_, timeout);

    } else if (keepAlive_) {
      events_ |= (EPOLLIN | EPOLLET);
      // events_ |= (EPOLLIN | EPOLLET | EPOLLONESHOT);
      int timeout = DEFAULT_KEEP_ALIVE_TIME;
      loop_->updatePoller(channel_, timeout);
    } else {
      // cout << "close normally" << endl;
      // loop_->shutdown(channel_);
      // loop_->runInLoop(bind(&HttpData::handleClose, shared_from_this()));
      events_ |= (EPOLLIN | EPOLLET);
      // events_ |= (EPOLLIN | EPOLLET | EPOLLONESHOT);
      int timeout = (DEFAULT_KEEP_ALIVE_TIME >> 1);
      loop_->updatePoller(channel_, timeout);
    }
  } else if (!error_ && connectionState_ == H_DISCONNECTING &&
             (events_ & EPOLLOUT)) {
    events_ = (EPOLLOUT | EPOLLET);
  } else {
    // cout << "close with errors" << endl;
    loop_->runInLoop(bind(&HttpData::handleClose, shared_from_this()));
  }
}

URIState HttpData::parseURI() {
  string &str = inBuffer_;
  string cop = str;
  // 读到完整的请求行再开始解析请求
  size_t pos = str.find('\r', nowReadPos_);
  if (pos < 0) {
    return PARSE_URI_AGAIN;
  }
  // 去掉请求行所占的空间，节省空间
  string request_line = str.substr(0, pos);
  if (str.size() > pos + 1)
    str = str.substr(pos + 1);
  else
    str.clear();
  // Method
  int posGet = request_line.find("GET");
  int posPost = request_line.find("POST");
  int posHead = request_line.find("HEAD");

  if (posGet >= 0) {
    pos = posGet;
    method_ = METHOD_GET;
  } else if (posPost >= 0) {
    pos = posPost;
    method_ = METHOD_POST;
  } else if (posHead >= 0) {
    pos = posHead;
    method_ = METHOD_HEAD;
  } else {
    return PARSE_URI_ERROR;
  }

  // filename
  pos = request_line.find("/", pos);

  if (pos < 0) {
    fileName_ = "base.html";
    HTTPVersion_ = HTTP_11;
    return PARSE_URI_SUCCESS;
  } else {
    size_t _pos = request_line.find(' ', pos);
    if (_pos < 0)
      return PARSE_URI_ERROR;
    else {
      if (_pos - pos > 1) {
        fileName_ = request_line.substr(pos + 1, _pos - pos - 1);
        size_t __pos = fileName_.find('?');
        if (__pos >= 0) {
          fileName_ = fileName_.substr(0, __pos);
        }
      }

      else
        fileName_ = "base.html";
    }
    pos = _pos;
  }
  // cout << "fileName_: " << fileName_ << endl;
  // HTTP 版本号
  pos = request_line.find("/", pos);
  if (pos < 0)
    return PARSE_URI_ERROR;
  else {
    if (request_line.size() - pos <= 3)
      return PARSE_URI_ERROR;
    else {
      string ver = request_line.substr(pos + 1, 3);
      if (ver == "1.0")
        HTTPVersion_ = HTTP_10;
      else if (ver == "1.1")
        HTTPVersion_ = HTTP_11;
      else
        return PARSE_URI_ERROR;
    }
  }
  return PARSE_URI_SUCCESS;
}

HeaderState HttpData::parseHeaders() {
  string &str = inBuffer_;
  int key_start = -1, key_end = -1, value_start = -1, value_end = -1;
  int now_read_line_begin = 0;
  bool notFinish = true;
  size_t i = 0;
  for (; i < str.size() && notFinish; ++i) {
    switch (hState_) {
      case H_START: {
        if (str[i] == '\n' || str[i] == '\r') break;
        hState_ = H_KEY;
        key_start = i;
        now_read_line_begin = i;
        break;
      }
      case H_KEY: {
        if (str[i] == ':') {
          key_end = i;
          if (key_end - key_start <= 0) return PARSE_HEADER_ERROR;
          hState_ = H_COLON;
        } else if (str[i] == '\n' || str[i] == '\r')
          return PARSE_HEADER_ERROR;
        break;
      }
      case H_COLON: {
        if (str[i] == ' ') {
          hState_ = H_SPACES_AFTER_COLON;
        } else
          return PARSE_HEADER_ERROR;
        break;
      }
      case H_SPACES_AFTER_COLON: {
        hState_ = H_VALUE;
        value_start = i;
        break;
      }
      case H_VALUE: {
        if (str[i] == '\r') {
          hState_ = H_CR;
          value_end = i;
          if (value_end - value_start <= 0) return PARSE_HEADER_ERROR;
        } else if (i - value_start > 255)
          return PARSE_HEADER_ERROR;
        break;
      }
      case H_CR: {
        if (str[i] == '\n') {
          hState_ = H_LF;
          string key(str.begin() + key_start, str.begin() + key_end);
          string value(str.begin() + value_start, str.begin() + value_end);
          headers_[key] = value;
          now_read_line_begin = i;
        } else
          return PARSE_HEADER_ERROR;
        break;
      }
      case H_LF: {
        if (str[i] == '\r') {
          hState_ = H_END_CR;
        } else {
          key_start = i;
          hState_ = H_KEY;
        }
        break;
      }
      case H_END_CR: {
        if (str[i] == '\n') {
          hState_ = H_END_LF;
        } else
          return PARSE_HEADER_ERROR;
        break;
      }
      case H_END_LF: {
        notFinish = false;
        key_start = i;
        now_read_line_begin = i;
        break;
      }
    }
  }
  if (hState_ == H_END_LF) {
    str = str.substr(i-1);

    //todo 
    if(str.size()>0) parseBody(str);
    return PARSE_HEADER_SUCCESS;
  }
  str = str.substr(now_read_line_begin);
  return PARSE_HEADER_AGAIN;
}
//新增加的 http body请求 ----------begin---------------------
bool HttpData::parseBody(string &text) {
    string processd = "";
    string strleft = text;
    while (true)
    {
        processd = strleft.substr(0, strleft.find("&"));
        m_map[processd.substr(0, processd.find("="))] = processd.substr(processd.find("=") + 1);
        strleft = strleft.substr(strleft.find("&") + 1);
        if (strleft == processd)
            break;
    }
    return true;
}
//新增加的 http body请求 ----------end---------------------


AnalysisState HttpData::analysisRequest() {

//新增加的 http网页post请求 ----------begin---------------------
//analysisRequest：end---->消息相应制作开始-----------------------
  if (method_ == METHOD_POST) {
    string header;
    string filename;
    string postmsg="";
    header += "HTTP/1.1 200 OK\r\n";

    redis_clt *m_redis = redis_clt::getinstance();
        if (fileName_ == "base.html" || fileName_ == "") //如果来自于登录界面
        {
            //处理登录请求
            //cout << "处理登录请求" << endl;
            //cout << "user's passwd: " << m_redis->getUserpasswd(m_map["username"]) << endl;
            //cout << "we got : " << m_map["passwd"] << endl;
            if (m_redis->getUserpasswd(m_map["username"]) == m_map["passwd"])
            {
                //cout << "登录进入欢迎界面" << endl;
                if (m_redis->getUserpasswd(m_map["username"]) == "root")
                    filename = "./pages/welcomeroot.html"; //登录进入欢迎界面
                else
                    filename = "./pages/welcome.html"; //登录进入欢迎界面
            }
            else
            {
                //cout << "登录失败界面" << endl;
                filename = "./pages/error.html"; //进入登录失败界面
            }
        }
        else if (fileName_ == "register.html") //如果来自注册界面
        {
            m_redis->setUserpasswd(m_map["username"], m_map["passwd"]);
            //cout << "set:" << m_map["username"]<<"passwd:  "<<m_map["passwd"] << endl;
            filename = "./pages/register.html"; //注册后进入初始登录界面
        }
        else if (fileName_== "welcome.html") //如果来自登录后界面
        {
            //cout << "get a vote ask:  "<<m_map["votename"] << endl;
            m_redis->vote(m_map["votename"]);
            
            //postmsg = "";
            return ANALYSIS_SUCCESS;
            //filename = "./pages/welcome.html"; //进入初始登录界面
        }
        else if (fileName_ == "getvote") //如果主页要请求投票
        {
            //读取redis
            //cout << "get read vote !" << endl;

            postmsg = m_redis->getvoteboard();
            //cout << postmsg << endl;
            
        }
        else
        {
            filename = "./pages/base.html"; //进入初始登录界面
        }
        /*body=------------------------------------------------*/
        if (postmsg != ""){
           
            if(postmsg.length() < 20) cout <<"wrong pstmsg : "<< postmsg << endl;
                
            header += "Content-Type: html\r\n";
            header += "Content-Length: " + to_string(strlen(postmsg.c_str())) + "\r\n";
            header += "Server: hhss's Web Server\r\n";
            // 头部结束
            header += "\r\n";
            outBuffer_ += header;
            outBuffer_ +=postmsg;
            
            
        }else{
            int fd = open(filename.c_str(), O_RDONLY);
            stat(filename.c_str(), &m_file_stat);
            char *file_content = (char *)mmap(0, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
            
            header += "Content-Type: html\r\n";
            header += "Content-Length: " + to_string(strlen(file_content)) + "\r\n";
            header += "Server: hhss's Web Server\r\n";
            // 头部结束
            header += "\r\n";
            outBuffer_ += header;
            outBuffer_ +=file_content;
        
        }
                /*body=------------------------------------------*/
        
        
        
        

//新增加的 http网页 post请求 ----------end---------------------
    return ANALYSIS_SUCCESS;
    
  } else if (method_ == METHOD_GET || method_ == METHOD_HEAD) {
  //新增加的 http网页 get请求 ----------begin---------------------
    string header;
    string filename;
    header += "HTTP/1.1 200 OK\r\n";
    if (headers_.find("Connection") != headers_.end() &&
        (headers_["Connection"] == "Keep-Alive" ||
         headers_["Connection"] == "keep-alive")) {
      keepAlive_ = true;
      header += string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" +
                to_string(DEFAULT_KEEP_ALIVE_TIME) + "\r\n";
    }
   filename = "./pages/";
   if (fileName_ == "base.html"||fileName_==""){
        filename+="base.html";
    }else{//如果来自注册界面
          filename += fileName_; //注册后进入初始登录界面
    }
   
    
    int fd = open(filename.c_str(), O_RDONLY);
    if(fd<0) {
        handleError(fd_, 400, "Bad Request");
        return ANALYSIS_ERROR;
    } 

    stat(filename.c_str(), &m_file_stat);
    char *file_content = (char *)mmap(0, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(fileName_.substr(fileName_.find('.')+1)=="png"){
        header += "Content-Type: png\r\n";
    }else{
        header += "Content-Type: html\r\n";
    }
    //cout<<fileName_<<" length is "<<m_file_stat.st_size<<endl;
    header += "Content-Length: " + to_string(m_file_stat.st_size) + "\r\n";
    header += "Server: hhss's Web Server\r\n";
    // 头部结束
    header += "\r\n";
    outBuffer_ += header;

    outBuffer_ +=string(file_content,m_file_stat.st_size);

  //新增加的 http网页get请求 ----------end---------------------
  
  
  //analysisRequest：end---->消息相应制作结束------
  return ANALYSIS_SUCCESS;
  }
}

void HttpData::handleError(int fd, int err_num, string short_msg) {
  short_msg = " " + short_msg;
  char send_buff[4096];
  string body_buff, header_buff;
  body_buff += "<html><title>访问出错了～</title>";
  body_buff += "<body bgcolor=\"ffffff\">";
  body_buff += to_string(err_num) + short_msg;
  body_buff += "<hr><em> hhss's Web Server</em>\n</body></html>";

  header_buff += "HTTP/1.1 " + to_string(err_num) + short_msg + "\r\n";
  header_buff += "Content-Type: text/html\r\n";
  header_buff += "Connection: Close\r\n";
  header_buff += "Content-Length: " + to_string(body_buff.size()) + "\r\n";
  header_buff += "Server: hhss's Web Server\r\n";
  ;
  header_buff += "\r\n";
  // 错误处理不考虑writen不完的情况
  sprintf(send_buff, "%s", header_buff.c_str());
  writen(fd, send_buff, strlen(send_buff));
  sprintf(send_buff, "%s", body_buff.c_str());
  writen(fd, send_buff, strlen(send_buff));
}

void HttpData::handleClose() {
  connectionState_ = H_DISCONNECTED;
  shared_ptr<HttpData> guard(shared_from_this());
  loop_->removeFromPoller(channel_);
}

void HttpData::newEvent() {
  channel_->setEvents(DEFAULT_EVENT);
  loop_->addToPoller(channel_, DEFAULT_EXPIRED_TIME);
}
